#include "MidiReader.h"
#include "MidiFile.h"
#include "MidiEventList.h"
#include "MidiEvent.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cctype>

MidiReader::MidiReader() {
  std::vector<std::string> names = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  int octave = -1;
  int note_number = 0;
  std::stringstream name;
  for(note_number = 0, octave = -2; note_number < 128; note_number++) {
    if(note_number % 12 == 0) {
      octave++;
    }
    name.str("");
    name << names[note_number%12] << octave;
    mNoteNumbersToNames[note_number] = name.str();
  }
}

MidiReader::~MidiReader() {
}

void MidiReader::readScore(std::istream& is, MusicalScore& score) const {
  smf::MidiFile midifile;
  midifile.read(is);
  midifile.doTimeAnalysis();
  midifile.linkNotePairs();
  readTiming(midifile, score);

  std::cout << "TPQ: " << midifile.getTicksPerQuarterNote() << std::endl;

  int tracks = midifile.getTrackCount();
  if(tracks > 1) std::cout << "TRACKS: " << tracks << std::endl;

  if(midifile.getTrackCount() < 2) {
    /* 
     * Assumes first track is timing track.
     */
    std::stringstream ss;
    ss << "Midifile has < 2 tracks.";
    throw std::invalid_argument(ss.str());
  }
  for(int track=1; track<tracks; track++) {
    if(trackHasNotes(midifile, track)) {
      readTrack(midifile, track, score);
      continue;
    }
    if(tracks > 1) std::cout << "\nTrack " << track << std::endl;

    std::cout << "Tick\tSeconds\tDur\tMessage" << std::endl;
    for (int event=0; event<midifile[track].size(); event++) {
      if (midifile[track][event].isNoteOff()) {
        continue;
      }
      if (midifile[track][event].isSustain()) {
        continue;
      }
      std::cout << std::dec << midifile[track][event].tick;
      std::cout << '\t' << std::dec << midifile[track][event].seconds;
      std::cout << '\t';
      if (midifile[track][event].isNoteOn())
        std::cout << midifile[track][event].getDurationInSeconds();
      std::cout << '\t' << std::hex;
      for (unsigned int i=0; i<midifile[track][event].size(); i++)
        std::cout << (int)midifile[track][event][i] << ' ';

      if(midifile[track][event].isText()) {
        std::string text = midifile[track][event].getMetaContent();
        std::cout << "TEXT : " << std::dec << text ;
      }
      if(midifile[track][event].isCopyright()) {
        std::string text = midifile[track][event].getMetaContent();
        std::cout << "COPYRIGHT : " << std::dec << text ;
      }
      if(midifile[track][event].isTrackName()) {
        std::string text = midifile[track][event].getMetaContent();
        std::cout << "TRACK-NAME: " << std::dec << text ;
      }
      if(midifile[track][event].isInstrumentName()) {
        std::string text = midifile[track][event].getMetaContent();
        std::cout << "INSTRUMENT-NAME: " << std::dec << text ;
      }
      if(midifile[track][event].isLyricText()) {
        std::string text = midifile[track][event].getMetaContent();
        std::cout << "LYRIC-TEXT: " << std::dec << text ;
      }
      if(midifile[track][event].isMarkerText()) {
        std::string text = midifile[track][event].getMetaContent();
        std::cout << "MARKER-TEXT: " << std::dec << text ;
      }
      if(midifile[track][event].isMetaMessage() &&
         midifile[track][event].getMetaType() == 0x21) {
        int device_id = midifile[track][event].getP3();
        std::cout << "DEVICE: " << std::dec << device_id ;
      }
      if(midifile[track][event].isMetaMessage() &&
         midifile[track][event].getMetaType() == 0x7f) {
        std::string text = midifile[track][event].getMetaContent();
        std::cout << "Specific: " << std::dec << text ;
      }
      if(midifile[track][event].isEndOfTrack()) {
        std::cout << "EOT" ;
      }


      if(midifile[track][event].isController()) {
        int number = midifile[track][event].getControllerNumber();
        int value = midifile[track][event].getControllerValue();

        if(midifile[track][event].isSustainOn()) {
          std::cout << "SUSTAIN-ON ";
        } else if(midifile[track][event].isSustainOff()) {
          std::cout << "SUSTAIN-OFF ";
        } else if(midifile[track][event].isSustain()) {
          std::cout << "SUSTAIN ";
        } else {
          std::cout << "CONTROLLER ";
        }

        std::cout << std::dec << number << " " << value ;
      }

      if(midifile[track][event].isPatchChange()) {
        int instrument = midifile[track][event].getP1();
        std::cout << "PATCH-CHANGE " << std::dec << instrument ;
      }
      if(midifile[track][event].isNoteOn()) {
        int note_number = midifile[track][event].getP1();
        int base7, accidental;
        midifile[track][event].getSpelling(base7, accidental);
        std::cout << "NOTE: " 
                  << midifile[track][event].seconds << " "
                  << midifile[track][event].getDurationInSeconds() << " "
                  << std::dec
                  << base7 << " "
                  << accidental << " "
                  << mNoteNumbersToNames.find(note_number)->second;
      }

      std::cout << std::endl;
    }
  }
  
}

void MidiReader::readTiming(smf::MidiFile& midifile, MusicalScore& score) const {
  /* Assumes that Track 0 is the timing track.
   * Grabs 
   *   the FIRST TimeSignature
   *   the FIRST Tempo
   * Assigns them to the MusicalScore
   */
  int track = 0;
  if(midifile.getTrackCount() < 1) {
    std::stringstream ss;
    ss << "Midifile has < 1 tracks.";
    throw std::invalid_argument(ss.str());
  }
  int event;
  bool first_time = true;
  bool first_tempo = true;
  for(event = 0; event < midifile[track].size(); event++) {
    
    if(first_time && midifile[track][event].isTimeSignature()) {
      int nn = (int)midifile[track][event][3];
      int dd = (int)midifile[track][event][4];
      int beat_value = 1;
      int i;
      for(i = 0; i < dd; i++) {
        beat_value *= 2;
      }
      score.getTimeSignature().setBeatsPerBar(nn);
      score.getTimeSignature().setBeatValue(beat_value);
      
      // int cc = (int)midifile[track][event][5];
      // int bb = (int)midifile[track][event][6];
      // std::cout << "TIME:SIG: "
      //           << std::dec
      //           << nn << "/" 
      //           << dd << " "
      //           << cc << " "
      //           << bb ;
      first_time = false;
    }
    if(first_tempo && midifile[track][event].isTempo()) {
      double BPM = midifile[track][event].getTempoBPM();
      score.setTempo(BPM);
      std::cout << "TEMPO: " << std::dec << BPM << " bpm " ;
      std::cout << "TPQ: " << std::dec << midifile.getTPQ() << " tpq" ;
      std::cout << std::endl;
      first_tempo = false;
    }
    // if(midifile[track][event].isKeySignature()) {
    //   int key = (int)midifile[track][event][3];
    //   int minor = (int)midifile[track][event][4];
    //   std::cout << "KEY: " << std::dec << key << " " << minor ;
    // }
  }

}

bool MidiReader::trackHasNotes(smf::MidiFile& midifile, const int track) const {
  if(midifile.getTrackCount() < track + 1) {
    std::stringstream ss;
    ss << "Midifile has < " << track + 1 << " tracks.";
    throw std::invalid_argument(ss.str());
  }
  bool result = false;
  smf::MidiEventList& event_list = midifile[track];
  int i;
  for(i = 0; i < event_list.size(); i++) {
    smf::MidiEvent& event = event_list[i];
    if(event.isNote()) {
      result = true;
      break;
    }
  }
  return result;
}

void MidiReader::readTrack(smf::MidiFile& midifile, const int track, MusicalScore& score) const {
  if(midifile.getTrackCount() < track + 1) {
    std::stringstream ss;
    ss << "Midifile has < " << track + 1 << " tracks.";
    throw std::invalid_argument(ss.str());
  }

  unsigned int staff_index = score.createStaff();
  MusicalStaff& staff = score.getStaff(staff_index);
  
  smf::MidiEventList& event_list = midifile[track];
  int i;
  for(i = 0; i < event_list.size(); i++) {
    smf::MidiEvent& event = event_list[i];
    if(event.isTrackName()) {
      std::string track_name = event.getMetaContent();
      std::string spaceless;
      for(unsigned int i = 0; i < track_name.size(); i++) {
        if(!std::isspace(track_name[i])) {
          spaceless += track_name[i];
        } else {
          spaceless += '_';
        }
      }
      staff.setName(spaceless);
    } else if(event.isNoteOn()) {
      // FIXME: need to convert duration from seconds to Note duration
      // event.tick;
      double start = event.seconds;
      double duration = event.getDurationInSeconds();
      duration = QUARTER_NOTE;
      int note_number = event.getP1();
      std::string note_name = mNoteNumbersToNames.find(note_number)->second;
      Note note(note_name, duration);
      StaffNote staff_note(note, start);
      staff.addNote(staff_note);
      std::cout << staff_note << std::endl;
      /*
        int base7, accidental;
        event.getSpelling(base7, accidental);
        std::cout << "NOTE: " 
                << event.seconds << " "
                << event.getDurationInSeconds() << " "
                << std::dec
                << base7 << " "
                << accidental << " "
                << mNoteNumbersToNames.find(note_number)->second;
        */
      
    } else if(event.isNoteOff()) {
      // skip
    } else if(event.isPatchChange()) {
      int instrument_number = event.getP1();
      Instrument *instrument = score.getInstrumentarium().getInstrument(instrument_number);
      staff.setInstrument(instrument);
    } else if(event.isSustain()) {
      // FIXME: skip sustain for now
    } else {
      std::cout << "OUCH: ";
      displayEventText(event);
    }
    
    
  }
  
}


void MidiReader::displayEventText(smf::MidiEvent& event) const {
  std::cout << std::dec << event.tick;
  std::cout << '\t' << std::dec << event.seconds;
  std::cout << '\t';

  if(event.isNoteOn()) {
    std::cout << event.getDurationInSeconds();
  }
  std::cout << '\t' << std::hex;
  for(unsigned int i=0; i<event.size(); i++) {
    std::cout << (int)event[i] << ' ';
  }
  
  if(event.isNoteOn()) {
    std::cout << "NOTE-ON : " << std::dec;
  }
  if(event.isNoteOff()) {
    std::cout << "NOTE-OFF : " << std::dec;
  }
  if(event.isText()) {
    std::string text = event.getMetaContent();
    std::cout << "TEXT : " << std::dec << text ;
  }
  if(event.isCopyright()) {
    std::string text = event.getMetaContent();
    std::cout << "COPYRIGHT : " << std::dec << text ;
  }
  if(event.isTrackName()) {
    std::string text = event.getMetaContent();
    std::cout << "TRACK-NAME: " << std::dec << text ;
  }
  if(event.isInstrumentName()) {
    std::string text = event.getMetaContent();
    std::cout << "INSTRUMENT-NAME: " << std::dec << text ;
  }
  if(event.isLyricText()) {
    std::string text = event.getMetaContent();
    std::cout << "LYRIC-TEXT: " << std::dec << text ;
  }
  if(event.isMarkerText()) {
    std::string text = event.getMetaContent();
    std::cout << "MARKER-TEXT: " << std::dec << text ;
  }
  if(event.isMetaMessage() &&
     event.getMetaType() == 0x21) {
    int device_id = event.getP3();
    std::cout << "DEVICE: " << std::dec << device_id ;
  }
  if(event.isMetaMessage() &&
     event.getMetaType() == 0x7f) {
    std::string text = event.getMetaContent();
    std::cout << "Specific: " << std::dec << text ;
  }
  if(event.isEndOfTrack()) {
    std::cout << "EOT" ;
  }
  
  if(event.isController()) {
    int number = event.getControllerNumber();
    int value = event.getControllerValue();
    
    if(event.isSustainOn()) {
      std::cout << "SUSTAIN-ON ";
    } else if(event.isSustainOff()) {
      std::cout << "SUSTAIN-OFF ";
    } else if(event.isSustain()) {
      std::cout << "SUSTAIN ";
    } else {
      std::cout << "CONTROLLER ";
    }
    
    std::cout << std::dec << number << " " << value ;
  }
  
  if(event.isPatchChange()) {
    int instrument = event.getP1();
    std::cout << "PATCH-CHANGE " << std::dec << instrument ;
  }
  if(event.isNoteOn()) {
    int note_number = event.getP1();
    int base7, accidental;
    event.getSpelling(base7, accidental);
    std::cout << "NOTE: " 
              << event.seconds << " "
              << event.getDurationInSeconds() << " "
              << std::dec
              << base7 << " "
              << accidental << " "
              << mNoteNumbersToNames.find(note_number)->second;
  }
  
  std::cout << std::endl;
}
