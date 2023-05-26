#ifndef _SQUAREWAVEFORM_H_
#define _SQUAREWAVEFORM_H_
#include "Waveform.h"
#include <vector>

class SquareWaveform: public Waveform {
public:
  SquareWaveform();
  virtual ~SquareWaveform();
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const;
  virtual SquareWaveform* clone() const;
protected:
private:
};



#endif /* _SQUAREWAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
