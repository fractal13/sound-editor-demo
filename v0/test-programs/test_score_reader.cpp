#include "MusicalScore.h"
#include "WaveFile.h"
#include "ScoreReader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  int bits_per_sample = 16;
  int samples_per_second = 44100;    // samples per second
  if(argc > 1) {
    std::stringstream arg(argv[1]);
    arg >> bits_per_sample;
  }
  std::string filename = "../doc/mary-had-a-little-lamb.score";
  if(argc > 2) {
    std::stringstream arg(argv[2]);
    arg >> filename;
  }
  std::string output_filename = "mary-had-a-little-lamb.wav";
  if(argc > 3) {
    std::stringstream arg(argv[3]);
    arg >> output_filename;
  }

  ScoreReader reader;
  MusicalScore score;
  std::ifstream f(filename);
  reader.readScore(f, score);
  f.close();
  
  unsigned int i;
  std::vector<std::vector<double>> values;
  for(i = 0; i < score.getNumberOfStaves(); i++) {
    values.push_back(std::vector<double>());
    score.renderStaff(i, samples_per_second, values[i]);
  }

  WaveFile wave(output_filename, samples_per_second, bits_per_sample);
  wave.writeHeader();
  wave.writeDataSubchunkHeader();
  wave.writeValues(values);
  wave.writeSizes();
  wave.close();

  return 0;
}
