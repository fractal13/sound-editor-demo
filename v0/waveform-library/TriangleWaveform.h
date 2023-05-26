#ifndef _TRIANGLEWAVEFORM_H_
#define _TRIANGLEWAVEFORM_H_
#include "Waveform.h"
#include <vector>

class TriangleWaveform: public Waveform {
public:
  TriangleWaveform();
  virtual ~TriangleWaveform();
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const;
  virtual TriangleWaveform* clone() const;
protected:
private:
};


#endif /* _TRIANGLEWAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
