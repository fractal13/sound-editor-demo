#ifndef _ADENVELOPE_H_
#define _ADENVELOPE_H_
#include "ADSREnvelope.h"
#include <vector>

class ADEnvelope: public ADSREnvelope {
public:
  ADEnvelope();
  ADEnvelope(const double attack_seconds);
  virtual ~ADEnvelope();
  virtual void generateAmplitudes(const double seconds, const int samples_per_second, std::vector<double>& amplitudes) const;
protected:
private:
};

#endif /* _ADENVELOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
