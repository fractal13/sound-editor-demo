#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_
#include <vector>

class Envelope {
public:
  Envelope(const double max_amplitude, const double attack_seconds, const double decay_seconds, const double sustain_amplitude, const double release_amplitude);
  void generateAmplitudes(std::vector<double>& amplitudes, const double seconds, const int samples_per_second) const;
private:
  double mMaximumAmplitude;
  // Attack, Decay, Sustain, Release
  double mAttackSeconds;
  double mDecaySeconds;
  double mSustainAmplitude;
  double mReleaseSeconds;
};


#endif /* _ENVELOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
