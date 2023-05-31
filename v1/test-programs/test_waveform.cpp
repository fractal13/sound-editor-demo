#include "SineWaveform.h"
#include "SawtoothWaveform.h"
#include "SquareWaveform.h"
#include "TriangleWaveform.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>

int main(int argc, char *argv[]) {
  Waveform *f = 0;
  if(argc > 1) {
    std::stringstream arg(argv[1]);
    std::string waveform;
    arg >> waveform;
    if(waveform == "sine") {
      f = new SineWaveform();
    } else if(waveform == "sawtooth") {
      f = new SawtoothWaveform();
    } else if(waveform == "square") {
      f = new SquareWaveform();
    } else if(waveform == "triangle") {
      f = new TriangleWaveform();
    } else {
      std::stringstream ss;
      ss << "Unknown waveform name: '" << waveform << "'.";
      throw std::invalid_argument(ss.str());
    }
  }
  std::vector<double> samples;
  double frequency = 440.00;
  double seconds = 1.0/frequency;
  int samples_per_second = 44100;
  f->generateSamples(frequency, seconds, samples_per_second, samples);
  
  unsigned int i;
  std::cout << "sample_number" << "," << "amplitude" << std::endl;
  for(i = 0; i < samples.size(); i++) {
    std::cout << i << "," << samples[i] << std::endl;
  }

  delete f;
  
  return 0;
}
