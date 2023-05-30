#include "SineWaveform.h"
#include <iostream>
#include <vector>

int main() {
  
  SineWaveform f;
  std::vector<double> samples;
  double frequency = 440.00;
  double seconds = 1.0/frequency;
  int samples_per_second = 44100;
  f.generateSamples(frequency, seconds, samples_per_second, samples);
  
  unsigned int i;
  std::cout << "sample_number" << "," << "amplitude" << std::endl;
  for(i = 0; i < samples.size(); i++) {
    std::cout << i << "," << samples[i] << std::endl;
  }
  
  return 0;
}
