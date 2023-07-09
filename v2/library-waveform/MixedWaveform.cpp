#include "MixedWaveform.h"
#include <cmath>

MixedWaveform::MixedWaveform(const std::string& name)
  : Waveform(name, "mixed") {
}

MixedWaveform::~MixedWaveform() {
}

double MixedWaveform::generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const {
  double value = 0.0;
  unsigned int i;
  for(i = 0; i < mInputs.size(); i++) {
    value += mInputs[i]->generateOneSample(frequency, sample_number, samples_per_second);
  }
  value *= mAmplitude;
  return value;
}

MixedWaveform* MixedWaveform::clone() const {
  auto copy = new MixedWaveform(mName);
  *copy = *this;
  copy->mInputs.resize(0);
  unsigned int i;
  for(i = 0; i < mInputs.size(); i++) {
    copy->addWaveform(mInputs[i]->clone());
  }
  return copy;
}

void MixedWaveform::addWaveform(Waveform *input) {
  mInputs.push_back(input);
}
