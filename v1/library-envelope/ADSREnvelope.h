#ifndef _ADSRENVELOPE_H_
#define _ADSRENVELOPE_H_
#include "Envelope.h"
#include <vector>

class ADSREnvelope: public Envelope {
public:
  ADSREnvelope();
  ADSREnvelope(const double attack_seconds, const double decay_seconds, const double sustain_amplitude, const double release_seconds);
virtual ~ADSREnvelope();
  virtual void generateAmplitudes(const double seconds, const int samples_per_second, std::vector<double>& amplitudes) const;

  void assignAttackAmplitudes(const int begin, const int end, std::vector<double>& amplitudes, const double a0=0.0, const double a1=1.0) const;
  void assignDecayAmplitudes(const int begin, const int end, std::vector<double>& amplitudes) const;
  void assignSustainAmplitudes(const int begin, const int end, std::vector<double>& amplitudes) const;  
  void assignReleaseAmplitudes(const int begin, const int end, std::vector<double>& amplitudes) const;
  virtual ADSREnvelope* clone() const;

protected:
  // Attack, Decay, Sustain, Release
  double mAttackSeconds;
  double mDecaySeconds;
  double mSustainAmplitude;
  double mReleaseSeconds;
private:
};

#endif /* _ADSRENVELOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
