#include "ADSREnvelope.h"
#include <iostream>
#include <vector>

int main() {

  ADSREnvelope e(0.1, 0.2, 0.5, 0.3);
  std::vector<double> amplitudes;
  e.generateAmplitudes(1.0, 100, amplitudes);
  
  unsigned int i;
  for(i = 0; i < amplitudes.size(); i++) {
    std::cout << "a[" << i << "] = " << amplitudes[i] << std::endl;
  }
  
  return 0;
}
