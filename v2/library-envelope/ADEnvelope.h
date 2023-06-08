#ifndef _ADENVELOPE_H_
#define _ADENVELOPE_H_
#include "ADSREnvelope.h"
#include <vector>

class ADEnvelope: public ADSREnvelope {
public:
  ADEnvelope(const std::string& name);
  ADEnvelope(const std::string& name, const double maximum_amplitude, const double attack_seconds);
  virtual ~ADEnvelope();
  virtual void generateAmplitudes(const double seconds, const int samples_per_second, AudioTrack& track) const;
  virtual ADEnvelope* clone() const;
protected:
private:
};

#endif /* _ADENVELOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
