#include "MusicalScore.h"
#include "WaveFile.h"
#include "ScoreReader.h"
#include "AudioTrack.h"
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
  std::vector<AudioTrack> tracks;
  for(i = 0; i < score.getNumberOfStaves(); i++) {
    tracks.push_back(AudioTrack());
    score.renderStaff(i, samples_per_second, tracks[i]);
  }

  unsigned int j;
  std::cout << "sample_number" << "," << "amplitude" << std::endl;
  for(j = 0; j < tracks[0].size(); j++) {
    std::cout << j << "," << tracks[0][j] << std::endl;
  }

  return 0;
}
