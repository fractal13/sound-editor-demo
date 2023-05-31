#include "ScoreReader.h"
#include "SineWaveform.h"
#include "SawtoothWaveform.h"
#include "SquareWaveform.h"
#include "TriangleWaveform.h"
#include "ADSREnvelope.h"
#include "ADEnvelope.h"
#include "AREnvelope.h"
#include <sstream>
#include <string>
#include <stdexcept>

ScoreReader::ScoreReader() {
}

ScoreReader::~ScoreReader() {
}

void ScoreReader::readScore(std::istream& is, MusicalScore& score) const {
  std::string word = "";
  int         integer;
  while(is) {
    is >> word;
    if(!is) { break; }

    if(word == "SCORE") {
      is >> integer;
      score.getTimeSignature().setBeatsPerBar(integer);
      is >> integer;
      score.getTimeSignature().setBeatValue(integer);
      is >> integer;
      score.setTempo(integer);
    } else if(word == "STAFF") {
      unsigned int staff_index = score.createStaff();
      MusicalStaff& staff = score.getStaff(staff_index);
      readStaff(is, score, staff);
    } else if(word == "INSTRUMENT") {
      Instrument *instrument = readInstrument(is, score);
      if(instrument) {
        delete instrument;
      }
    } else if(word == "SCORE-END") {
      break;
    } else {
      std::stringstream ss;
      ss << "Expected a keyword, but got: '" << word << "'.";
      throw std::invalid_argument(ss.str());
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
    std::stringstream ss;
    ss << "Expected an instrument name, but got: '" << instrument_name << "'.";
    throw std::invalid_argument(ss.str());
  }
  staff.setName(name);
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

void ScoreReader::parseNote(const std::string& word, Note& note) const {
  // qE4, w.Bb2, etc
  double extension = 1.0;
  unsigned int first_note = 1;

  if(word[1] == '.') {
    extension = 1.5;
    first_note = 2;
  }

  switch(word[0]) {
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
        std::stringstream ss;
        ss << "Unknown instrument parameter: '" << word << "'.";
        throw std::invalid_argument(ss.str());
      }
      is >> word;
    }
    instrument = new Instrument(name, waveform, envelope);
    score.getInstrumentarium().addInstrument(name, instrument->clone());
  }

  // Check end
  if(word != "INSTRUMENT-END") {
    delete instrument;
    std::stringstream ss;
    ss << "Expected 'INSTRUMENT-END' but got: '" << word << "'.";
    throw std::invalid_argument(ss.str());
  }

  return instrument;
}

Waveform* ScoreReader::readWaveform(std::istream& is, MusicalScore& score) const {
  /* // WAVEFORM has already been read
    WAVEFORM name type
    WAVEFORM-END
   */
  Waveform* waveform = 0;
  std::string name, type, end;
  is >> name >> type >> end;

  // Check end
  if(end != "WAVEFORM-END") {
    std::stringstream ss;
    ss << "Expected 'WAVEFORM-END' but got: '" << end << "'.";
    throw std::invalid_argument(ss.str());
  }

  // Lookup if exists
  waveform = score.getWaveforms().getWaveform(name);
  
  if(!waveform) {
    // Check type
    if(type == "sine") {
      waveform = new SineWaveform();
    } else if(type == "sawtooth") {
      waveform = new SawtoothWaveform();
    } else if(type == "square") {
      waveform = new SquareWaveform();
    } else if(type == "triangle") {
      waveform = new TriangleWaveform();
    } else {
      std::stringstream ss;
      ss << "Unknown waveform type: '" << type << "'.";
      throw std::invalid_argument(ss.str());
    }

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
      std::stringstream ss;
      ss << "Unknown envelope type: '" << type << "'.";
      throw std::invalid_argument(ss.str());
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
        std::stringstream ss;
        ss << "Unknown envelope parameter: '" << word << "'.";
        throw std::invalid_argument(ss.str());
      }
    }

    if(type == "ADSR") {
      envelope = new ADSREnvelope(maximum_amplitude, attack_seconds, decay_seconds, sustain_amplitude, release_seconds);
    } else if(type == "AD") {
      envelope = new ADEnvelope(maximum_amplitude, attack_seconds);
    } else if(type == "AR") {
      envelope = new AREnvelope(maximum_amplitude, attack_seconds, sustain_amplitude, release_seconds);
    } else {
      std::stringstream ss;
      ss << "Unknown envelope type: '" << type << "'.";
      throw std::invalid_argument(ss.str());
    }
    
    // Store, if didn't exist.
    score.getEnvelopes().addEnvelope(name, envelope->clone());
  }

  // Check end
  if(word != "ENVELOPE-END") {
    delete envelope;
    std::stringstream ss;
    ss << "Expected 'ENVELOPE-END' but got: '" << word << "'.";
    throw std::invalid_argument(ss.str());
  }
  
  return envelope;
}
