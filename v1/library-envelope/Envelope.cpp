#include "Envelope.h"

Envelope::Envelope() 
  : mMaximumAmplitude(1.0) {
}

Envelope::Envelope(const double amplitude) 
  : mMaximumAmplitude(amplitude) {
}

Envelope::~Envelope() {
}

double Envelope::getMaximumAmplitude() const {
  return mMaximumAmplitude;
}

void Envelope::setMaximumAmplitude(const double amplitude) {
  if(amplitude > 0.0) {
    mMaximumAmplitude = amplitude;
  }
}
