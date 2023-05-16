#include "Envelope.h"
#include <vector>
#include <exception>

Envelope::Envelope(const double max_amplitude, const double attack_seconds, const double decay_seconds, const double sustain_amplitude, const double release_seconds)
  : mMaximumAmplitude(max_amplitude), mAttackSeconds(attack_seconds), mDecaySeconds(decay_seconds), mSustainAmplitude(sustain_amplitude), mReleaseSeconds(release_seconds) {
}

void Envelope::generateAmplitudes(std::vector<double>& amplitudes, const double seconds, const int samples_per_second) const {
  if(seconds < mAttackSeconds + mDecaySeconds + mReleaseSeconds) {
    throw std::exception();
  }
  int N = samples_per_second * seconds;  // total number of samples
  amplitudes.resize(N);
  int attack_n = samples_per_second * mAttackSeconds;
  int decay_n = attack_n + samples_per_second * mDecaySeconds;
  int sustain_n = N - samples_per_second * mReleaseSeconds;
  int release_n = N;
  int i;
  double m, x, b;

  // attack from 0 to full
  b = 0.0;
  m = mMaximumAmplitude / (attack_n - 0);
  for(i = 0; i < attack_n; i++) {
    x = i;
    amplitudes[i] = m*x+b;
  }

  // decay from full to sustain
  b = mMaximumAmplitude;
  m = (mSustainAmplitude - mMaximumAmplitude) / (decay_n - attack_n);
  for(i = attack_n; i < decay_n; i++) {
    x = i-attack_n;
    amplitudes[i] = m*x+b;
  }
  
  // sustain
  b = mSustainAmplitude;
  m = 0;
  for(i = decay_n; i < sustain_n; i++) {
    x = i-decay_n;
    amplitudes[i] = m*x+b;
  }
  
  // decay from sustain to 0.0 (release)
  b = mSustainAmplitude;
  m = (0 - mSustainAmplitude) / (release_n - sustain_n);
  for(i = sustain_n; i < release_n; i++) {
    x = i-sustain_n;
    amplitudes[i] = m*x+b;
  }
}

