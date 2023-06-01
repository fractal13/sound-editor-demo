#include "AREnvelope.h"
#include <sstream>
#include <stdexcept>

AREnvelope::AREnvelope(const std::string& name)
  : ADSREnvelope(name, "AR", 1.0, 0.0, 0.0, 0.0, 0.0) {
}

AREnvelope::AREnvelope(const std::string& name, const double maximum_amplitude, const double attack_seconds, const double sustain_amplitude, const double release_seconds)
  : ADSREnvelope(name, "AR", maximum_amplitude, attack_seconds, 0.0, sustain_amplitude, release_seconds) {
}

AREnvelope::~AREnvelope() {
}

void AREnvelope::generateAmplitudes(const double seconds, const int samples_per_second, AudioTrack& track) const {

  if(seconds < mAttackSeconds + mReleaseSeconds) {
    std::stringstream ss;
    ss << "Envelope size is too small for sum of all stages." << std::endl;
    ss << "seconds: " << seconds << std::endl;
    ss << "attack seconds: " << mAttackSeconds << std::endl;
    ss << "release seconds: " << mReleaseSeconds << std::endl;
    throw std::invalid_argument(ss.str());
  }
  track.setSize(samples_per_second, seconds);

  // indices of last sample in envelope region
  int attack_n  = samples_per_second * mAttackSeconds;
  int release_n = track.getSampleCount();
  int sustain_n = release_n - samples_per_second * mReleaseSeconds;

  // attack from 0 to sustain
  assignAttackAmplitudes(0, attack_n, track, 0.0, mSustainAmplitude);

  // sustain
  assignSustainAmplitudes(attack_n, sustain_n, track, mSustainAmplitude);

  // decay from sustain to 0.0 (release)
  assignReleaseAmplitudes(sustain_n, release_n, track, mSustainAmplitude, 0.0);
}

AREnvelope* AREnvelope::clone() const {
  auto copy = new AREnvelope(mName);
  *copy = *this;
  return copy;
}
