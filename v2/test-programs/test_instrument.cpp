#include "EasyInstrument.h"
#include "AudioTrack.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  std::string waveform = "sine";
  std::string envelope = "ADSR";

  if(argc > 1) {
    std::stringstream arg(argv[1]);
    arg >> waveform;
  }
  if(argc > 2) {
    std::stringstream arg(argv[2]);
    arg >> envelope;
  }

  Instrument *instrument = new EasyInstrument(waveform, envelope);

  AudioTrack track;
  double frequency = 440.00;
  double seconds = 0.10;
  int samples_per_second = 44100;

  instrument->generateSamples(frequency, seconds, samples_per_second, track);
  
  unsigned int i;
  std::cout << "sample_number" << "," << "amplitude" << std::endl;
  for(i = 0; i < track.size(); i++) {
    std::cout << i << "," << track[i] << std::endl;
  }

  delete instrument;

  return 0;
}
