#ifndef _WAVEFILE_H_
#define _WAVEFILE_H_
#include "Note.h"
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

class WaveFile {
public:
  WaveFile(const std::string& filename, int samples_per_second, int bits_per_sample);
  ~WaveFile();

  void writeHeader();
  void writeDataSubchunkHeader();
  void writeNotes(const std::vector<Note>& notes);
  void writeValues(const std::vector<std::vector<double>>& values);
  void writeSizes();
  void close();

private:
  std::ofstream mFile;
  int mSamplesPerSecond;
  size_t mDataSubchunkPosition;
  int mBitsPerSample;
  int mMaximumAmplitude;
  int mBytesPerSample;
};



#endif /* _WAVEFILE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
