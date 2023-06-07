#include "MidiReader.h"
#include "MidiFile.h"
#include "MidiEventList.h"
#include "MidiEvent.h"
#include "debug.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
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

  std::cout << "TPQ: " << midifile.getTicksPerQuarterNote() << std::endl;

  readTiming(midifile, score);

  int tracks = midifile.getTrackCount();
  if(tracks > 1) std::cout << "TRACKS: " << tracks << std::endl;

  if(midifile.getTrackCount() < 2) {
    /* 
     * Assumes first track is timing track.
     */
    DEBUG_INVALID("Midifile has < 2 tracks. '" << midifile.getTrackCount() << "' is less than 2.");
  }
  for(int track=1; track<tracks; track++) {
    if(trackHasNotes(midifile, track)) {
      readTrack(midifile, track, score);
    } else {
      // FIXME what to do with noteless tracks?
    }
    /*
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
    */
  }
  addMixer(score);
}

void MidiReader::readTiming(smf::MidiFile& midifile, MusicalScore& score) const {
  /* Assumes that Track 0 is the timing track. (FIXME)
   * Grabs 
   *   the FIRST TimeSignature
   *   the FIRST Tempo
   * Assigns them to the MusicalScore
   */
  if(midifile.getTrackCount() < 1) {
    DEBUG_INVALID("Midifile has < 1 tracks.");
  }
  int track = 0;
  bool first_time = true;
  bool first_tempo = true;
  smf::MidiEventList& event_list = midifile[track];
  int i;
  for(i = 0; i < event_list.size(); i++) {
    smf::MidiEvent& event = event_list[i];
    
    if(first_time && event.isTimeSignature()) {
      int nn = (int)event[3];
      int dd = (int)event[4];
      int beat_value = 1;
      int i;
      for(i = 0; i < dd; i++) {
        beat_value *= 2;
      }
      score.getTimeSignature().setBeatsPerBar(nn);
      score.getTimeSignature().setBeatValue(beat_value);
      first_time = false;
      displayEventText(event);
    }
    if(first_tempo && event.isTempo()) {
      double BPM = event.getTempoBPM();
      score.setTempo(BPM);
      first_tempo = false;
      displayEventText(event);
    }
  }

}

bool MidiReader::trackHasNotes(smf::MidiFile& midifile, const int track) const {
  if(midifile.getTrackCount() < track + 1) {
    DEBUG_INVALID("Midifile has < " << track + 1 << " tracks.");
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
    DEBUG_INVALID("Midifile has < " << track + 1 << " tracks.");
  }
  std::cout << "Track : " << track << std::endl;
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
      /*
        if a midi quarter note is 24 ticks; then
        whole note = 96 ticks
        half note = 48 ticks
        quarter note = 24 ticks
        eighth note =  12 ticks
        dotted eighth note =  18 ticks
        dotted quarter note =  36 ticks
        dotted half note =  72 ticks
      */

      double tpq = midifile.getTicksPerQuarterNote();
      double duration = QUARTER_NOTE * (event.getTickDuration() / tpq);
      
      /*
      switch(event.getTickDuration()) {
      case 12: duration = EIGHTH_NOTE; break;
      case 24: duration = QUARTER_NOTE; break;
      case 48: duration = HALF_NOTE; break;
      case 96: duration = WHOLE_NOTE; break;
      default:
        std::cout << "TD: " << event.getTickDuration() << " ";
        break;
      }
      */

      // FIXME: need to convert duration from seconds to Note duration
      // event.tick;
      double start = QUARTER_NOTE * event.tick / tpq;
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

  if(staff.getName() == "") {
    score.setUniqueStaffName(staff);
  }
  
}


void MidiReader::displayEventText(smf::MidiEvent& event) const {
  std::cout << "[ ";
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
  std::cout << " ] ";

  
  if(event.isNote()) {
    std::cout << "NOTE-";
    if(event.isNoteOn()) {
      std::cout << "ON : ";
      
      int note_number = event.getP1();
      int base7, accidental;
      event.getSpelling(base7, accidental);
      std::cout << event.seconds << " "
                << event.getDurationInSeconds() << " "
                << std::dec
                << base7 << " "
                << accidental << " "
                << mNoteNumbersToNames.find(note_number)->second;

    } else if(event.isNoteOff()) {
      std::cout << "OFF : ";
    } else {
      std::cout << std::endl;
      DEBUG_INVALID("Unexpected NOTE event.");
    }
  } else if(event.isController()) {
    int number = event.getControllerNumber();
    int value = event.getControllerValue();

    std::cout << "CONTROLLER-";
    
    if(event.isSustain()) {
      std::cout << "SUSTAIN-";
      if(event.isSustainOn()) {
        std::cout << "ON : ";
      } else if(event.isSustainOff()) {
        std::cout << "OFF : ";
      } else {
        std::cout << std::endl;
        DEBUG_INVALID("Unexpected SUSTAIN event.");
      }
    } else if(event.isSoft()) {
      std::cout << "SOFT-";
      if(event.isSoftOn()) {
        std::cout << "ON : ";
      } else if(event.isSoftOff()) {
        std::cout << "OFF : ";
      } else {
        std::cout << std::endl;
        DEBUG_INVALID("Unexpected SOFT event.");
      }
    } else if(number == 0x00) {
      std::cout << "BANK-SELECT : ";
    } else if(number == 0x01) {
      std::cout << "MODULATION-WHEEL : ";
    } else if(number == 0x06) {
      std::cout << "DATA-ENTRY-MSB : ";
    } else if(number == 0x07) {
      std::cout << "CHANNEL-VOLUME : ";
    } else if(number == 0x0A) {
      std::cout << "PAN : ";
    } else if(number == 0x0B) {
      std::cout << "EXPRESSION-CONTROLLER : ";
    } else if(number == 0x20) {
      std::cout << "LSB-BANK-SELECT : ";
    } else if((number & 0xF0) == 0x20) {
      std::cout << "LSB-CONTROL-" << std::dec << (number & 0xF) << " : ";
    } else if(number == 0x5B) {
      std::cout << "EFFECTS-1-DEPTH : ";
    } else if(number == 0x5D) {
      std::cout << "EFFECTS-3-DEPTH : ";
    } else if(number == 0x62) {
      std::cout << "NRPN-LSB : ";
    } else if(number == 0x63) {
      std::cout << "NRPN-MSB : ";
    } else if(number == 0x64) {
      std::cout << "RPN-LSB : ";
    } else if(number == 0x65) {
      std::cout << "RPN-MSB : ";
    } else if(number == 0x78) {
      std::cout << "ALL-SOUND-OFF : ";
    } else if(number == 0x79) {
      std::cout << "RESET-ALL-CONTROLLERS : ";
    } else if(number == 0x7B) {
      std::cout << "ALL-NOTES-OFF : ";
    } else {
      std::cout << std::endl;
      DEBUG_INVALID("Unexpected CONTROLLER event.");
    }
    std::cout << std::dec << number << " " << value;
  } else if(event.isPatchChange()) {
    // isPatchChange() and isTimbre() are the same
    int instrument = event.getP1();
    std::cout << "PATCH-CHANGE " << std::dec << instrument;
  } else if(event.isPitchbend()) {
    int p1 = event.getP1();
    std::cout << "PITCH-BEND " << std::dec << p1;
  } else if(event.isMetaMessage()) {

    if(event.isText()) {
      std::string text = event.getMetaContent();
      std::cout << "TEXT : " << std::dec << text;
    } else if(event.isCopyright()) {
      std::string text = event.getMetaContent();
      std::cout << "COPYRIGHT : " << std::dec << text;
    } else if(event.isTrackName()) {
      std::string text = event.getMetaContent();
      std::cout << "TRACK-NAME : " << std::dec << text;
    } else if(event.isInstrumentName()) {
      std::string text = event.getMetaContent();
      std::cout << "INSTRUMENT-NAME : " << std::dec << text;
    } else if(event.isLyricText()) {
      std::string text = event.getMetaContent();
      std::cout << "LYRIC-TEXT : " << std::dec << text;
    } else if(event.isMarkerText()) {
      std::string text = event.getMetaContent();
      std::cout << "MARKER-TEXT : " << std::dec << text;
    } else if(event.isTempo()) {
      double tpq = 120; // FIXME: 120 is not always correct
      //double tpmq = 24; // FIXME: 24 is not always correct
      //double tspmq = 8; // FIXME: 8 is not always correct
      double BPM = event.getTempoBPM();
      double TPS = event.getTempoTPS(tpq);
      double SPT = event.getTempoSPT(tpq);
      std::cout << "TEMPO : " << std::dec << BPM << " bpm ";
      std::cout << std::dec << TPS << " tps ";
      std::cout << std::dec << SPT << " spt ";
      
    } else if(event.isTimeSignature()) {
      int nn = (int)event[3];
      int dd = (int)event[4];
      int cc = (int)event[5]; // midi clocks per metronome click
      int bb = (int)event[6]; // notated 32nds per midi quarter
      int bottom = 1;
      while(dd--) {
        bottom *= 2;
      }
      std::cout << "TIME:SIG : "
                << std::dec
                << nn << "/" 
                << bottom << " "
                << cc << " "
                << bb;
    } else if(event.isKeySignature()) {
      int key = (int)event[3];
      int minor = (int)event[4];
      std::cout << "KEY : " << std::dec << key << " " << minor;
    } else if(event.isEndOfTrack()) {
      std::cout << "EOT" ;
    } else if(event.isMetaMessage() &&
              event.getMetaType() == 0x21) {
      int device_id = event.getP3();
      std::cout << "DEVICE: " << std::dec << device_id ;
    } else if(event.isMetaMessage() &&
              event.getMetaType() == 0x7f) {
      std::string text = event.getMetaContent();
      std::cout << "Specific: " << std::dec << text ;
    } else {
      std::cout << std::endl;
      DEBUG_INVALID("Unexpected META event.");
    }
  } else if(event.getCommandByte() == 0xf0){
    std::cout << "System Exclusive: ";
  } else {
    std::cout << std::endl;
    DEBUG_INVALID("Unexpected event.");
  }
  
  std::cout << std::endl;
}

void MidiReader::addMixer(MusicalScore& score) const {
  unsigned int staff_count = score.getNumberOfStaves();
  unsigned int staff_number;
  double staff_weight = 1.0/staff_count;

  Mixer mixer;
  int channel_number;
  for(channel_number = 0; channel_number < 2; channel_number++) {
    Channel channel;
    channel.setChannelNumber(channel_number);
    for(staff_number = 0; staff_number < staff_count; staff_number++) {
      channel.addStaff(score.getStaff(staff_number).getName(), staff_weight);
    }
    mixer.addChannel(channel);
  }

  score.getMixer() = mixer;
}
