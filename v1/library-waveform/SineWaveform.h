#ifndef _SINEWAVEFORM_H_
#define _SINEWAVEFORM_H_
#include "Waveform.h"
#include <vector>

class SineWaveform:  public Waveform {
public:
  SineWaveform(const std::string& name);
  virtual ~SineWaveform();
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const;
  virtual SineWaveform* clone() const;
protected:
private:
};



#endif /* _SINEWAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
