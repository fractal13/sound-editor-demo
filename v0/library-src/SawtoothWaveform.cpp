#include "SawtoothWaveform.h"
#include <cmath>

SawtoothWaveform::SawtoothWaveform() {
}

SawtoothWaveform::~SawtoothWaveform() {
}

double SawtoothWaveform::generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const {
  double cycle_position = computeSampleCyclePosition(frequency, sample_number, samples_per_second);
  double value = 0.0;
  if(cycle_position < 0.5) {
    value = 2.0 * cycle_position + 0.0;
  } else {
    value = 2.0 * (cycle_position - 0.5) - 1.0;
  }
  return value;
}

SawtoothWaveform* SawtoothWaveform::clone() const {
  auto copy = new SawtoothWaveform;
  *copy = *this;
  return copy;
}
