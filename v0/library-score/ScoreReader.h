#ifndef _SCOREREADER_H_
#define _SCOREREADER_H_

#include "MusicalScore.h"
#include <iostream>

class ScoreReader {
public:
  ScoreReader();
  virtual ~ScoreReader();

  void readScore(std::istream& is, MusicalScore& score) const;
  void readStaff(std::istream& is, MusicalScore& score, MusicalStaff& staff) const;
  Instrument* readInstrument(std::istream& is, MusicalScore& score) const;
  Waveform* readWaveform(std::istream& is, MusicalScore& score) const;
  Envelope* readEnvelope(std::istream& is, MusicalScore& score) const;
  void parseNote(const std::string& word, Note& note) const;
protected:
private:
};

#endif /* _SCOREREADER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
