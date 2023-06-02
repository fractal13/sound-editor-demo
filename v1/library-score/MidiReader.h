#ifndef _MIDIREADER_CPP_
#define _MIDIREADER_CPP_

#include "MusicalScore.h"
#include "Mixer.h"
#include "MidiFile.h"
#include <iostream>
#include <map>
#include <string>

class MidiReader {
public:
  MidiReader();
  virtual ~MidiReader();

  void readScore(std::istream& is, MusicalScore& score) const;
  void readTiming(smf::MidiFile& midifile, MusicalScore& score) const;
protected:
  std::map<int, std::string> mNoteNumbersToNames;
private:
};

#endif /* _MIDIREADER_CPP_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
