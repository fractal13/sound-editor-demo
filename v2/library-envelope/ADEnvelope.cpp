#include "ADEnvelope.h"
#include <sstream>
#include <stdexcept>

ADEnvelope::ADEnvelope(const std::string& name)
  : ADSREnvelope(name, "AD", 1.0, 0.0, 0.0, 0.0, 0.0) {
}

ADEnvelope::ADEnvelope(const std::string& name, const double maximum_amplitude, const double attack_seconds)
  : ADSREnvelope(name, "AD", maximum_amplitude, attack_seconds, 0.0, 0.0, 0.0) {
}

ADEnvelope::~ADEnvelope() {
}

void ADEnvelope::generateAmplitudes(const double seconds, const int samples_per_second, AudioTrack& track) const {

  if(seconds < mAttackSeconds) {
    std::stringstream ss;
    ss << "Envelope size is too small for sum of all stages." << std::endl;
    ss << "seconds: " << seconds << std::endl;
    ss << "attack seconds: " << mAttackSeconds << std::endl;
    throw std::invalid_argument(ss.str());
  }
  track.setSize(samples_per_second, seconds);

  // indices of last sample in envelope region
  int attack_n  = samples_per_second * mAttackSeconds;
  int decay_n   = track.getSampleCount();

  // attack from 0 to full
  assignAttackAmplitudes(0, attack_n, track, 0.0, mMaximumAmplitude);

  // decay from full to sustain
  assignDecayAmplitudes(attack_n, decay_n, track, mMaximumAmplitude, mSustainAmplitude);
}

ADEnvelope* ADEnvelope::clone() const {
  auto copy = new ADEnvelope(mName);
  *copy = *this;
  return copy;
}
