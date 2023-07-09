#include "ScoreReader.h"
#include "SineWaveform.h"
#include "SawtoothWaveform.h"
#include "SquareWaveform.h"
#include "TriangleWaveform.h"
#include "FMSynthesisWaveform.h"
#include "MixedWaveform.h"
#include "ADSREnvelope.h"
#include "ADEnvelope.h"
#include "AREnvelope.h"
#include "debug.h"
#include <string>

ScoreReader::ScoreReader() {
}

ScoreReader::~ScoreReader() {
}

void ScoreReader::readScore(std::istream& is, MusicalScore& score) const {
  std::string word = "";
  int         integer;
  double      number;
  while(is) {
    is >> word;
    if(!is) { break; }

    if(word == "SCORE") {
      is >> integer;
      score.getTimeSignature().setBeatsPerBar(integer);
      is >> integer;
      score.getTimeSignature().setBeatValue(integer);
      is >> number;
      score.setTempo(number);
    } else if(word == "STAFF") {
      unsigned int staff_index = score.createStaff();
      MusicalStaff& staff = score.getStaff(staff_index);
      readStaff(is, score, staff);
    } else if(word == "INSTRUMENT") {
      Instrument *instrument = readInstrument(is, score);
      if(instrument) {
        delete instrument;
      }
    } else if(word == "MIXER") {
      readMixer(is, score, score.getMixer());
    } else if(word == "SCORE-END") {
      break;
    } else {
      DEBUG_INVALID("Expected a keyword, but got: '" << word << "'.");
    }
  }
  
}

void ScoreReader::readStaff(std::istream& is, MusicalScore& score, MusicalStaff& staff) const {
  // assumes that STAFF keyword has been consumed
  // reads staff header, then notes until STAFF-END is encountered
  std::string name, instrument_name;

  Instrument *instrument = 0;
  is >> name; // staff name, FIXME
  is >> instrument_name;
  instrument = score.getInstrumentarium().getInstrument(instrument_name);
  if(!instrument) {
    DEBUG_INVALID("Expected an instrument name, but got: '" << instrument_name << "'.");
  }
  //std::cout << "staff read name: '" << name << "'" << std::endl;
  staff.setName(name);
  //std::cout << "staff stored name: '" << staff.getName() << "'" << std::endl;
  staff.setInstrument(instrument);

  std::string word;
  while(is >> word) {
    if(word == "STAFF-END") {
      break;
    }
    std::stringstream wstream;
    double start;
    wstream.str(word);
    wstream >> start;
    is >> word;
    Note note;
    parseNote(word, note);
    StaffNote staff_note(note, start);
    staff.addNote(staff_note);
  }

}

void ScoreReader::readMixer(std::istream& is, MusicalScore& score, Mixer& mixer) const {
  /* assumes MIXER keyword has already been consumed
MIXER
  CHANNEL 0
    STAFF piano1 0.7
    STAFF bass1 0.3
  CHANNEL-END

  CHANNEL 1
    STAFF piano2 0.7
    STAFF bass1 0.3
  CHANNEL-END
MIXER-END
   */
  std::string word;
  while(is >> word && word != "MIXER-END") {
    if(word == "CHANNEL") {
      Channel channel;
      readChannel(is, score, channel);
      mixer.addChannel(channel);
    } else {
      DEBUG_INVALID("Expected a CHANNEL clause, but got: '" << word << "'.");
    }
  }
}
void ScoreReader::readChannel(std::istream& is, MusicalScore& score, Channel& channel) const {
  /* assumes CHANNEL keyword has already been consumed
  CHANNEL 0
    STAFF piano1 0.7
    STAFF bass1 0.3
  CHANNEL-END
   */
  (void) score;
  std::string word, staff_name;
  int channel_number;
  double weight;
  is >> channel_number;
  channel.setChannelNumber(channel_number);
  while(is >> word && word != "CHANNEL-END") {
    if(word == "STAFF") {
      is >> staff_name >> weight;
      channel.addStaff(staff_name, weight);
    } else {
      DEBUG_INVALID("Expected a STAFF clause, but got: '" << word << "'.");
    }
  }
}

void ScoreReader::parseNote(const std::string& word, Note& note) const {
  // qE4, w.Bb2, etc
  double extension = 1.0;
  unsigned int first_note = 1;

  if(word[1] == '.') {
    extension = 1.5;
    first_note = 2;
  } else if(word[1] == 't') {
    extension = 1.0/3.0;
    first_note = 2;
  }

  switch(word[0]) {
  case 's':
    note.setDuration(SIXTEENTH_NOTE*extension);
    break;
  case 'e':
    note.setDuration(EIGHTH_NOTE*extension);
    break;
  case 'q':
    note.setDuration(QUARTER_NOTE*extension);
    break;
  case 'h':
    note.setDuration(HALF_NOTE*extension);
    break;
  case 'w':
    note.setDuration(WHOLE_NOTE*extension);
    break;
  default:
    DEBUG_INVALID("Expected a known note letter, but got: '" << word[0] << "'.");
    break;
  }
  
  note.setName(word.substr(first_note));
}

Instrument* ScoreReader::readInstrument(std::istream& is, MusicalScore& score) const {
  /* // INSTRUMENT has already been read
    INSTRUMENT piano-left
      WAVEFORM piano-sawtooth sawtooth
      WAVEFORM-END
    
      ENVELOPE piano-adsr ADSR
        MAXIMUM-AMPLITUDE 0.25
        ATTACK-SECONDS 0.01
        DECAY-SECONDS 0.02
        SUSTAIN-AMPLITUDE 0.5
        RELEASE-SECONDS 0.03
      ENVELOPE-END
    INSTRUMENT-END
   */
  
  Instrument *instrument = 0;
  Waveform *waveform = 0;
  Envelope *envelope = 0;
  std::string name, word;
  is >> name >> word;

  // Lookup if exists
  instrument = score.getInstrumentarium().getInstrument(name);

  if(!instrument) {
    while(word != "INSTRUMENT-END") {
      if(word == "WAVEFORM") {
        waveform = readWaveform(is, score);
      } else if(word == "ENVELOPE") {
        envelope = readEnvelope(is, score);
      } else {
        DEBUG_INVALID("Unknown instrument parameter: '" << word << "'.");
      }
      is >> word;
    }
    instrument = new Instrument(name, waveform, envelope);
    score.getInstrumentarium().addInstrument(name, instrument->clone());
  }

  // Check end
  if(word != "INSTRUMENT-END") {
    delete instrument;
    DEBUG_INVALID("Expected 'INSTRUMENT-END' but got: '" << word << "'.");
  }

  return instrument;
}

Waveform* ScoreReader::readWaveform(std::istream& is, MusicalScore& score) const {
  /* // WAVEFORM has already been read
    WAVEFORM name type
      BETA number
      FREQUENCY-RATIO number
    WAVEFORM-END
   */
  bool exists = false;
  Waveform* waveform = 0;
  std::string name, type, word;
  double number;
  is >> name >> type >> word;

  // Lookup if exists
  waveform = score.getWaveforms().getWaveform(name);
  
  if(!waveform) {
    // Check type
    if(type == "sine") {
      waveform = new SineWaveform(name);
    } else if(type == "sawtooth") {
      waveform = new SawtoothWaveform(name);
    } else if(type == "square") {
      waveform = new SquareWaveform(name);
    } else if(type == "triangle") {
      waveform = new TriangleWaveform(name);
    } else if(type == "fm-synthesis") {
      waveform = new FMSynthesisWaveform(name);
    } else if(type == "mixed") {
      waveform = new MixedWaveform(name);
    } else {
      DEBUG_INVALID("Unknown waveform type: '" << type << "'.");
    }
  } else {
    exists = true;
  }
  
  while(word != "WAVEFORM-END") {
    if(!exists) {
      if(word == "WAVEFORM-END") {
        continue;
      } else if(word == "BETA") {
        is >> number;
        FMSynthesisWaveform *w = dynamic_cast<FMSynthesisWaveform *>(waveform);
        if(w) {
          w->setBeta(number);
        }
      } else if(word == "FREQUENCY-RATIO") {
        is >> number;
        FMSynthesisWaveform *w = dynamic_cast<FMSynthesisWaveform *>(waveform);
        if(w) {
          w->setFrequencyRatio(number);
        }
      } else if(word == "AMPLITUDE") {
        is >> number;
        waveform->setAmplitude(number);
      } else if(word == "FREQUENCY-OFFSET") {
        is >> number;
        waveform->setFrequencyOffset(number);
      } else if(word == "WAVEFORM") {
        Waveform *sub_waveform = readWaveform(is, score);
        MixedWaveform *w = dynamic_cast<MixedWaveform *>(waveform);
        if(w) {
          w->addWaveform(sub_waveform);
        }
      } else {
        DEBUG_INVALID("Unknown waveform parameter: '" << word << "'.");
      }
    }
    is >> word;
  }

  // Check end
  if(word != "WAVEFORM-END") {
    DEBUG_INVALID("Expected 'WAVEFORM-END' but got: '" << word << "'.");
  }

  if(!exists) {
    // Store, if didn't exist.
    score.getWaveforms().addWaveform(name, waveform->clone());
  }

  return waveform;
}

Envelope* ScoreReader::readEnvelope(std::istream& is, MusicalScore& score) const {

  /* // ENVELOPE has already been read
    ENVELOPE piano-adsr ADSR
      MAXIMUM-AMPLITUDE 0.25
      ATTACK-SECONDS 0.01
      DECAY-SECONDS 0.02
      SUSTAIN-AMPLITUDE 0.5
      RELEASE-SECONDS 0.03
    ENVELOPE-END
   */
  Envelope* envelope = 0;
  std::string name, type, word;
  is >> name >> type >> word;

  // Lookup if exists
  envelope = score.getEnvelopes().getEnvelope(name);
  
  if(!envelope) {
    // Check type
    if(type == "ADSR") {
    } else if(type == "AD") {
    } else if(type == "AR") {
    } else {
      DEBUG_INVALID("Unknown envelope type: '" << type << "'.");
    }

    double maximum_amplitude = 1.0;
    double attack_seconds = 0.0;
    double decay_seconds = 0.0;
    double sustain_amplitude = 0.0;
    double release_seconds = 0.0;
    while(word != "ENVELOPE-END") {
      if(word == "MAXIMUM-AMPLITUDE") {
        is >> maximum_amplitude >> word;
      } else if(word == "ATTACK-SECONDS") {
        is >> attack_seconds >> word;
      } else if(word == "ATTACK-SECONDS") {
        is >> attack_seconds >> word;
      } else if(word == "DECAY-SECONDS") {
        is >> decay_seconds >> word;
      } else if(word == "SUSTAIN-AMPLITUDE") {
        is >> sustain_amplitude >> word;
      } else if(word == "RELEASE-SECONDS") {
        is >> release_seconds >> word;
      } else {
        DEBUG_INVALID("Unknown envelope parameter: '" << word << "'.");
      }
    }

    if(type == "ADSR") {
      envelope = new ADSREnvelope(name, maximum_amplitude, attack_seconds, decay_seconds, sustain_amplitude, release_seconds);
    } else if(type == "AD") {
      envelope = new ADEnvelope(name, maximum_amplitude, attack_seconds);
    } else if(type == "AR") {
      envelope = new AREnvelope(name, maximum_amplitude, attack_seconds, sustain_amplitude, release_seconds);
    } else {
      DEBUG_INVALID("Unknown envelope type: '" << type << "'.");
    }
    
    // Store, if didn't exist.
    score.getEnvelopes().addEnvelope(name, envelope->clone());
  } else {
    // read all words until envelope ends
    while(is && word != "ENVELOPE-END") {
      is >> word;
    }
    
  }

  // Check end
  if(word != "ENVELOPE-END") {
    delete envelope;
    DEBUG_INVALID("Expected 'ENVELOPE-END' but got: '" << word << "'.");
  }
  
  return envelope;
}
