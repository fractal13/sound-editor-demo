#include "ADSREnvelope.h"
#include <sstream>
#include <stdexcept>

ADSREnvelope::ADSREnvelope()
  : Envelope(), mAttackSeconds(0.0), mDecaySeconds(0.0), mSustainAmplitude(0.5), mReleaseSeconds(0.0) {
}

ADSREnvelope::ADSREnvelope(const double maximum_amplitude, const double attack_seconds, const double decay_seconds, const double sustain_amplitude, const double release_seconds)
  : Envelope(maximum_amplitude), mAttackSeconds(attack_seconds), mDecaySeconds(decay_seconds), mSustainAmplitude(sustain_amplitude), mReleaseSeconds(release_seconds) {
}

ADSREnvelope::~ADSREnvelope() {
}

void ADSREnvelope::generateAmplitudes(const double seconds, const int samples_per_second, AudioTrack& track) const {

  if(seconds < mAttackSeconds + mDecaySeconds + mReleaseSeconds) {
    std::stringstream ss;
    ss << "Envelope size is too small for sum of all stages." << std::endl;
    ss << "seconds: " << seconds << std::endl;
    ss << "attack seconds: " << mAttackSeconds << std::endl;
    ss << "decay seconds: " << mDecaySeconds << std::endl;
    ss << "release seconds: " << mReleaseSeconds << std::endl;
    throw std::invalid_argument(ss.str());
  }
  track.setSize(samples_per_second, seconds);

  // indices of last sample in envelope region
  int attack_n  = samples_per_second * mAttackSeconds;
  int decay_n   = attack_n + samples_per_second * mDecaySeconds;
  int release_n = track.getSampleCount();
  int sustain_n = release_n - samples_per_second * mReleaseSeconds;

  // attack from 0 to full
  assignAttackAmplitudes(0, attack_n, track, 0.0, mMaximumAmplitude);

  // decay from full to sustain
  assignDecayAmplitudes(attack_n, decay_n, track, mMaximumAmplitude, mSustainAmplitude);
  
  // sustain
  assignSustainAmplitudes(decay_n, sustain_n, track, mSustainAmplitude);
  
  // decay from sustain to 0.0 (release)
  assignReleaseAmplitudes(sustain_n, release_n, track, mSustainAmplitude, 0.0);
}

void ADSREnvelope::assignAttackAmplitudes(const int begin, const int end, AudioTrack& track, const double a0, const double a1) const {
  int i;
  double m, x, b;
  // attack from 0 to full
  b = a0;
  m = (a1-a0) / (end - begin);
  for(i = begin; i < end; i++) {
    x = i - begin;
    track[i] = m*x+b;
  }
}

void ADSREnvelope::assignDecayAmplitudes(const int begin, const int end, AudioTrack& track, const double a0, const double a1) const {
  int i;
  double m, x, b;
  // decay from full to sustain
  b = a0;
  m = (a1 - a0) / (end - begin);
  for(i = begin; i < end; i++) {
    x = i - begin;
    track[i] = m*x+b;
  }
}

void ADSREnvelope::assignSustainAmplitudes(const int begin, const int end, AudioTrack& track, const double a0) const {
  int i;
  double m, x, b;
  // sustain
  b = a0;
  m = 0;
  for(i = begin; i < end; i++) {
    x = i - begin;
    track[i] = m*x+b;
  }
}

void ADSREnvelope::assignReleaseAmplitudes(const int begin, const int end, AudioTrack& track, const double a0, const double a1) const {
  int i;
  double m, x, b;

  // decay from sustain to 0.0 (release)
  b = a0;
  m = (a1 - a0) / (end - begin);
  for(i = begin; i < end; i++) {
    x = i - begin;
    track[i] = m*x+b;
  }
  
}

ADSREnvelope* ADSREnvelope::clone() const {
  auto copy = new ADSREnvelope;
  *copy = *this;
  return copy;
}
