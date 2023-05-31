#include "ADEnvelope.h"
#include <sstream>
#include <stdexcept>

ADEnvelope::ADEnvelope()
  : ADSREnvelope(1.0, 0.0, 0.0, 0.0, 0.0) {
}

ADEnvelope::ADEnvelope(const double maximum_amplitude, const double attack_seconds)
  : ADSREnvelope(maximum_amplitude, attack_seconds, 0.0, 0.0, 0.0) {
}

ADEnvelope::~ADEnvelope() {
}

void ADEnvelope::generateAmplitudes(const double seconds, const int samples_per_second, std::vector<double>& amplitudes) const {

  if(seconds < mAttackSeconds) {
    std::stringstream ss;
    ss << "Envelope size is too small for sum of all stages." << std::endl;
    ss << "seconds: " << seconds << std::endl;
    ss << "attack seconds: " << mAttackSeconds << std::endl;
    throw std::invalid_argument(ss.str());
  }
  int N = samples_per_second * seconds;  // total number of samples
  amplitudes.resize(N);

  // indices of last sample in envelope region
  int attack_n  = samples_per_second * mAttackSeconds;
  int decay_n   = N;

  // attack from 0 to full
  assignAttackAmplitudes(0, attack_n, amplitudes, 0.0, mMaximumAmplitude);

  // decay from full to sustain
  assignDecayAmplitudes(attack_n, decay_n, amplitudes, mMaximumAmplitude, mSustainAmplitude);
}

ADEnvelope* ADEnvelope::clone() const {
  auto copy = new ADEnvelope;
  *copy = *this;
  return copy;
}
