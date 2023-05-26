#include "SquareWaveform.h"
#include <cmath>

SquareWaveform::SquareWaveform()
  : Waveform() {
}

SquareWaveform::~SquareWaveform() {
}

double SquareWaveform::generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const {
  
  double cycle_position = computeSampleCyclePosition(frequency, sample_number, samples_per_second);
  double value = 0.0;
  if(cycle_position < 0.5) {
    value = 1.0;
  } else {
    value = -1.0;
  }
  return value;
}

SquareWaveform* SquareWaveform::clone() const {
  auto copy = new SquareWaveform;
  *copy = *this;
  return copy;
}
