#ifndef _MIDIREADER_CPP_
#define _MIDIREADER_CPP_

#include "MusicalScore.h"
#include "MidiFile.h"
#include "MidiEvent.h"
#include <iostream>
#include <map>
#include <string>

class MidiReader {
public:
  MidiReader();
  virtual ~MidiReader();

  void readScore(std::istream& is, MusicalScore& score) const;
  void readTiming(smf::MidiFile& midifile, MusicalScore& score) const;
  bool trackHasNotes(smf::MidiFile& midifile, const int track) const;
  void readTrack(smf::MidiFile& midifile, const int track, MusicalScore& score) const;

  void displayEventText(smf::MidiEvent& event) const;
protected:
  std::map<int, std::string> mNoteNumbersToNames;
private:
};

#endif /* _MIDIREADER_CPP_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
