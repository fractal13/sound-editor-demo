#ifndef _ARENVELOPE_H_
#define _ARENVELOPE_H_
#include "ADSREnvelope.h"
#include <vector>

class AREnvelope: public ADSREnvelope {
public:
  AREnvelope(const std::string& name);
  AREnvelope(const std::string& name, const double maximum_amplitude, const double attack_seconds, const double sustain_amplitude, const double release_seconds);
  virtual ~AREnvelope();
  virtual void generateAmplitudes(const double seconds, const int samples_per_second, AudioTrack& track) const;
  virtual AREnvelope* clone() const;
protected:
private:
};

#endif /* _ARENVELOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
