#include "AREnvelope.h"
#include <iostream>
#include <vector>

int main() {

  AREnvelope e(0.3, 0.75, 0.2);
  std::vector<double> amplitudes;
  e.generateAmplitudes(1.0, 100, amplitudes);
  
  unsigned int i;
  std::cout << "sample_number" << "," << "amplitude" << std::endl;
  for(i = 0; i < amplitudes.size(); i++) {
    //std::cout << "a[" << i << "] = " << amplitudes[i] << std::endl;
    std::cout << i << "," << amplitudes[i] << std::endl;
  }
  
  return 0;
}
