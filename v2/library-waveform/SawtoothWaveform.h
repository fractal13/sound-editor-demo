#ifndef _SAWTOOTHWAVEFORM_H_
#define _SAWTOOTHWAVEFORM_H_
#include "Waveform.h"
#include <vector>

class SawtoothWaveform: public Waveform {
public:
  SawtoothWaveform(const std::string& name);
  virtual ~SawtoothWaveform();
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const;
  virtual SawtoothWaveform* clone() const;
protected:
private:
};

#endif /* _SAWTOOTHWAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
