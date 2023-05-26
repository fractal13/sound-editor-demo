#include "AREnvelope.h"
#include <sstream>
#include <stdexcept>

AREnvelope::AREnvelope()
  : ADSREnvelope(0.0, 0.0, 0.0, 0.0) {
}

AREnvelope::AREnvelope(const double attack_seconds, const double sustain_amplitude, const double release_seconds)
  : ADSREnvelope(attack_seconds, 0.0, sustain_amplitude, release_seconds) {
}

AREnvelope::~AREnvelope() {
}

void AREnvelope::generateAmplitudes(const double seconds, const int samples_per_second, std::vector<double>& amplitudes) const {

  if(seconds < mAttackSeconds + mReleaseSeconds) {
    std::stringstream ss;
    ss << "Envelope size is too small for sum of all stages." << std::endl;
    ss << "seconds: " << seconds << std::endl;
    ss << "attack seconds: " << mAttackSeconds << std::endl;
    ss << "release seconds: " << mReleaseSeconds << std::endl;
    throw std::invalid_argument(ss.str());
  }
  int N = samples_per_second * seconds;  // total number of samples
  amplitudes.resize(N);

  // indices of last sample in envelope region
  int attack_n  = samples_per_second * mAttackSeconds;
  int sustain_n = N - samples_per_second * mReleaseSeconds;
  int release_n = N;

  // attack from 0 to sustain
  assignAttackAmplitudes(0, attack_n, amplitudes, 0.0, mSustainAmplitude);

  // sustain
  assignSustainAmplitudes(attack_n, sustain_n, amplitudes);

  // decay from sustain to 0.0 (release)
  assignReleaseAmplitudes(sustain_n, release_n, amplitudes);
}
