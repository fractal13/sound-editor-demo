#include "MusicalScore.h"
#include "WaveFile.h"
#include "ScoreReader.h"
#include "ScoreWriter.h"
#include "AudioTrack.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  int bits_per_sample = 16;
  int samples_per_second = 44100;    // samples per second
  std::string input_filename = "../doc/mary-had-a-little-lamb.score";
  if(argc > 1) {
    std::stringstream arg(argv[1]);
    arg >> input_filename;
  }
  std::string output_filename = "mary-had-a-little-lamb.score";
  if(argc > 2) {
    std::stringstream arg(argv[2]);
    arg >> output_filename;
  }

  MusicalScore score;

  ScoreReader reader;
  std::ifstream fin(input_filename);
  reader.readScore(fin, score);
  fin.close();

  ScoreWriter writer;
  std::ofstream fout(output_filename);
  writer.writeScore(fout, score);
  fout.close();

  return 0;
}
