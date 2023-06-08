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
  std::map<std::string, AudioTrack> tracks;
  for(i = 0; i < score.getNumberOfStaves(); i++) {
    //std::cout << "staff["<<i<<"] name = " << score.getStaff(i).getName() << std::endl;
    //std::cout << "staff["<<i<<"] duration = " << score.getStaff(i).getDurationInWholeNotes() << std::endl;
    auto& track = tracks[score.getStaff(i).getName()] = AudioTrack();
    score.renderStaff(i, samples_per_second, track);
  }
  //  for(auto& track: tracks) {
  //    std::cout << "track: " << track.first << " " << track.second.size() << " " << track.second.getSeconds() << std::endl;
  //  }

  std::vector<AudioTrack> channels;
  score.getMixer().mix(tracks, channels);

  WaveFile wave(output_filename, samples_per_second, bits_per_sample);
  wave.writeHeader();
  wave.writeDataSubchunkHeader();
  wave.writeTracks(channels);
  wave.writeSizes();
  wave.close();

  return 0;
}
