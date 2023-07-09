#include "SawtoothWaveform.h"
#include <cmath>

SawtoothWaveform::SawtoothWaveform(const std::string& name)
  : Waveform(name, "sawtooth") {
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
  value *= mAmplitude;
  return value;
}

SawtoothWaveform* SawtoothWaveform::clone() const {
  auto copy = new SawtoothWaveform(mName);
  *copy = *this;
  return copy;
}
