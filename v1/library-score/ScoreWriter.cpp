#include "ScoreWriter.h"
#include "ADSREnvelope.h"
#include "ADEnvelope.h"
#include "AREnvelope.h"
#include <sstream>
#include <string>
#include <stdexcept>

ScoreWriter::ScoreWriter() {
}

ScoreWriter::~ScoreWriter() {
}

void ScoreWriter::writeScore(std::ostream& os, const MusicalScore& score) const {
  os << "SCORE" << " " << score.getTimeSignature().getBeatsPerBar() << " " << score.getTimeSignature().getBeatValue() << " " << score.getTempo() << std::endl;
  os << std::endl;
  for(const auto& instrument: score.getInstrumentarium()) {
    writeInstrument(os, score, *instrument.second);
  }
  
  unsigned int i;
  for(i = 0; i < score.getNumberOfStaves(); i++) {
    writeStaff(os, score, score.getStaff(i));
  }

  writeMixer(os, score, score.getMixer());
  
  os << std::endl;
  os << "SCORE-END" << std::endl;
}


void ScoreWriter::writeStaff(std::ostream& os, const MusicalScore& score, const MusicalStaff& staff) const {
  os << "  STAFF" << " " << staff.getName() << " " << staff.getInstrument().getName() << std::endl;
  for(const auto& note: staff.getNotes()) {
    writeStaffNote(os, score, note);
  }
  os << "  STAFF-END" << std::endl;
  os << std::endl;
}

void ScoreWriter::writeMixer(std::ostream& os, const MusicalScore& score, const Mixer& mixer) const {
  os << "  MIXER" << std::endl;
  for(const auto& channel: mixer.getChannels()) {
    writeChannel(os, score, channel);
  }
  os << "  MIXER-END" << std::endl;
  os << std::endl;
}

void ScoreWriter::writeChannel(std::ostream& os, const MusicalScore& score, const Channel& channel) const {
  (void) score;
  os << "    CHANNEL" << " " <<  channel.getChannelNumber() << std::endl;
  for(const auto& staff: channel.getStaves()) {
    os << "      STAFF " << staff.first << " " << staff.second << std::endl;
  }
  os << "    CHANNEL-END" << std::endl;
}


void ScoreWriter::writeInstrument(std::ostream& os, const MusicalScore& score, const Instrument& instrument) const {
  os << "  INSTRUMENT" << " " << instrument.getName() << std::endl;
  writeWaveform(os, score, *(instrument.getWaveform()));
  writeEnvelope(os, score, *(instrument.getEnvelope()));
  os << "  INSTRUMENT-END" << std::endl;
  os << std::endl;
}

void ScoreWriter::writeWaveform(std::ostream& os, const MusicalScore& score, const Waveform& waveform) const {
  (void) score;
  os << "    WAVEFORM" << " " << waveform.getName() << " " << waveform.getTypeName()  << std::endl;
  os << "    WAVEFORM-END" << std::endl;
}

void ScoreWriter::writeEnvelope(std::ostream& os, const MusicalScore& score, const Envelope& envelope) const {
  (void) score;
  os << "    ENVELOPE" << " " << envelope.getName() << " " << envelope.getTypeName()  << std::endl;
  os << "      MAXIMUM-AMPLITUDE " << envelope.getMaximumAmplitude() << std::endl;

  const ADSREnvelope *adsr = dynamic_cast<const ADSREnvelope*>(&envelope);
  const ADEnvelope *ad = dynamic_cast<const ADEnvelope*>(&envelope);
  const AREnvelope *ar = dynamic_cast<const AREnvelope*>(&envelope);
  // order matters due to inheritance
  if(ad) {
    os << "      ATTACK-SECONDS " << adsr->getAttackSeconds() << std::endl;
  } else if(ar) {
    os << "      ATTACK-SECONDS " << adsr->getAttackSeconds() << std::endl;
    os << "      SUSTAIN-AMPLITUDE " << adsr->getSustainAmplitude() << std::endl;
    os << "      RELEASE-SECONDS " << adsr->getReleaseSeconds() << std::endl;
  } else if(adsr) {
    os << "      ATTACK-SECONDS " << adsr->getAttackSeconds() << std::endl;
    os << "      DECAY-SECONDS " << adsr->getDecaySeconds() << std::endl;
    os << "      SUSTAIN-AMPLITUDE " << adsr->getSustainAmplitude() << std::endl;
    os << "      RELEASE-SECONDS " << adsr->getReleaseSeconds() << std::endl;
  } else {
    std::stringstream ss;
    ss << "Expected a known Envelope type, but got: '" << envelope.getTypeName() << "'.";
    throw std::invalid_argument(ss.str());
  }

  os << "    ENVELOPE-END" << std::endl;
}

void ScoreWriter::writeStaffNote(std::ostream& os, const MusicalScore& score, const StaffNote& staff_note) const {
  (void) score;
  os << "    " << staff_note.getStart() << " " << formatNote(staff_note.getNote()) << std::endl;;
}

std::string ScoreWriter::getDurationLetter(const double duration) const {
  std::map<std::string, double> durations = 
    {
     { "s", 1.0/16.0 },
     { "e", 1.0/8.0 },
     { "q", 1.0/4.0 },
     { "h", 1.0/2.0 },
     { "w", 1.0/1.0 },
     { "s.", 1.5/16.0 },
     { "e.", 1.5/8.0 },
     { "q.", 1.5/4.0 },
     { "h.", 1.5/2.0 },
     { "w.", 1.5/1.0 },
     { "st", 1.0/16.0/3.0 },
     { "et", 1.0/8.0/3.0 },
     { "qt", 1.0/4.0/3.0 },
     { "ht", 1.0/2.0/3.0 },
     { "wt", 1.0/1.0/3.0 },
  };
  std::string duration_letter = "X";
  for(auto& d: durations) {
    if(duration >= 0.65 * d.second && duration <= 1.50 * d.second) {
      duration_letter = d.first;
    }
  }
  if(duration_letter == "X") {
    if(duration < 0.015) { // FIXME
      duration_letter = "st";
    } else if(duration > 1.5) { // FIXME
      duration_letter = "w.";
    }
  }

  if(duration_letter == "X") {
    std::stringstream ss;
    ss << "Expected known note duration but got: '" << duration << "'." << " " << __FILE__ << ":" << __LINE__ << std::endl;
    for(auto& d: durations) {
      ss << d.first << " " << d.second << std::endl;
    }
    
    throw std::invalid_argument(ss.str());
  }

  return duration_letter;
}

std::string ScoreWriter::formatNote(const Note& note) const {
  std::stringstream ss;
  std::string duration_letter = getDurationLetter(note.getDuration());
  ss << duration_letter << note.getName();
  return ss.str();
}

