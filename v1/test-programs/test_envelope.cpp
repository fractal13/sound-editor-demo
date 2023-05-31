#include "ADEnvelope.h"
#include "AREnvelope.h"
#include "ADSREnvelope.h"
#include "AudioTrack.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>

int main(int argc, char *argv[]) {
  Envelope *e = 0;
  if(argc > 1) {
    std::stringstream arg(argv[1]);
    std::string envelope;
    arg >> envelope;
    if(envelope == "AD") {
      e = new ADEnvelope(1.0, 0.2);
    } else if(envelope == "AR") {
      e = new AREnvelope(1.0, 0.3, 0.75, 0.2);
    } else if(envelope == "ADSR") {
      e = new ADSREnvelope(1.0, 0.1, 0.2, 0.5, 0.3);
    } else {
      std::stringstream ss;
      ss << "Unknown envelope name: '" << envelope << "'.";
      throw std::invalid_argument(ss.str());
    }
  }
  AudioTrack track;
  e->generateAmplitudes(1.0, 100, track);

  unsigned int i;
  std::cout << "sample_number" << "," << "amplitude" << std::endl;
  for(i = 0; i < track.size(); i++) {
    std::cout << i << "," << track[i] << std::endl;
  }

  delete e;
  
  return 0;
}
