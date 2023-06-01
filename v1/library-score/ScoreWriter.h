#ifndef _SCOREWRITER_H_
#define _SCOREWRITER_H_

#include "MusicalScore.h"
#include "Mixer.h"
#include <iostream>

class ScoreWriter {
public:
  ScoreWriter();
  virtual ~ScoreWriter();

  void writeScore(std::ostream& os, const MusicalScore& score) const;
  void writeStaff(std::ostream& os, const MusicalScore& score, const MusicalStaff& staff) const;
  void writeMixer(std::ostream& os, const MusicalScore& score, const Mixer& mixer) const;
  void writeChannel(std::ostream& os, const MusicalScore& score, const Channel& channel) const;
  void writeInstrument(std::ostream& os, const MusicalScore& score, const Instrument& instrument) const;
  void writeWaveform(std::ostream& os, const MusicalScore& score, const Waveform& waveform) const;
  void writeEnvelope(std::ostream& os, const MusicalScore& score, const Envelope& envelope) const;
  void writeStaffNote(std::ostream& os, const MusicalScore& score, const StaffNote& staff_note) const;
  std::string formatNote(const Note& note) const;
protected:
private:
};

#endif /* _SCOREWRITER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
