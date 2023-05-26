#include "TriangleWaveform.h"
#include <cmath>

TriangleWaveform::TriangleWaveform()
  : Waveform() {
}

TriangleWaveform::~TriangleWaveform() {
}

double TriangleWaveform::generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const {

  double cycle_position = computeSampleCyclePosition(frequency, sample_number, samples_per_second);
  double value = 0.0;
  if(cycle_position < 0.25) {
    value = 4.0 * (cycle_position - 0.00) + 0.0;
  } else if(cycle_position < 0.75) {
    value = -4.0 * (cycle_position - 0.25) + 1.0;
  } else {
    value = 4.0 * (cycle_position - 0.75) - 1.0;
  }
  return value;
}

TriangleWaveform* TriangleWaveform::clone() const {
  auto copy = new TriangleWaveform;
  *copy = *this;
  return copy;
}
