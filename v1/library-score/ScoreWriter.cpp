#include "ScoreWriter.h"

ScoreWriter::ScoreWriter() {
}

ScoreWriter::~ScoreWriter() {
}

void ScoreWriter::writeScore(std::ostream& os, const MusicalScore& score) const {
  os << "SCORE" << " " << score.getTimeSignature().getBeatsPerBar() << " " << score.getTimeSignature().getBeatValue() << " " << score.getTempo() << std::endl;
  for(const auto& instrument: score.getInstrumentarium()) {
    writeInstrument(os, score, *instrument.second);
  }
  
  unsigned int i;
  for(i = 0; i < score.getNumberOfStaves(); i++) {
    writeStaff(os, score, score.getStaff(i));
  }

  writeMixer(os, score, score.getMixer());
  
  os << "SCORE-END" << std::endl;
}

/*
void ScoreWriter::writeStaff(std::ostream& os, const MusicalScore& score, const MusicalStaff& staff) const;
void ScoreWriter::writeMixer(std::ostream& os, const MusicalScore& score, const Mixer& mixer) const;
void ScoreWriter::writeChannel(std::ostream& os, const MusicalScore& score, const Channel& channel) const;
void ScoreWriter::writeInstrument(std::ostream& os, const MusicalScore& score, const Instrument& instrument) const;
void ScoreWriter::writeWaveform(std::ostream& os, const MusicalScore& score, const Waveform& waveform) const;
void ScoreWriter::writeEnvelope(std::ostream& os, const MusicalScore& score, const Envelope& envelope) const;
void ScoreWriter::writeNote(std::ostream& os, const MusicalScore& score, const Note& note) const;
*/
