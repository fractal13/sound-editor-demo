#include "SineWaveform.h"
#include <cmath>

SineWaveform::SineWaveform(const std::string& name)
  : Waveform(name, "sine") {
}

SineWaveform::~SineWaveform() {
}

double SineWaveform::generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const {
  double angle = computeSampleAngle(frequency, sample_number, samples_per_second);
  double value  = std::sin(angle);
  return value;
}

SineWaveform* SineWaveform::clone() const {
  auto copy = new SineWaveform(mName);
  *copy = *this;
  return copy;
}
