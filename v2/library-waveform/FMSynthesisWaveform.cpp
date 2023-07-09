#include "FMSynthesisWaveform.h"
#include <cmath>

FMSynthesisWaveform::FMSynthesisWaveform(const std::string& name)
  : Waveform(name, "FMSynthesis"), mBeta(1.0), mFrequencyRatio(1.0) {
}

FMSynthesisWaveform::~FMSynthesisWaveform() {
}

double FMSynthesisWaveform::generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const {
  double modulator_frequency = frequency / mFrequencyRatio;
  double modulator_angle = computeSampleAngle(modulator_frequency, sample_number, samples_per_second);
  double angle = computeSampleAngle(frequency, sample_number, samples_per_second);
  double value  = std::sin(angle + mBeta*modulator_angle);
  value *= mAmplitude;
  return value;
}

FMSynthesisWaveform* FMSynthesisWaveform::clone() const {
  auto copy = new FMSynthesisWaveform(mName);
  *copy = *this;
  return copy;
}

double FMSynthesisWaveform::getBeta() const {
  return mBeta;
}

double FMSynthesisWaveform::getFrequencyRatio() const {
  return mFrequencyRatio;
}

void FMSynthesisWaveform::setBeta(const double beta) {
  mBeta = beta;
}

void FMSynthesisWaveform::setFrequencyRatio(const double frequency_ratio) {
  mFrequencyRatio = frequency_ratio;
}
