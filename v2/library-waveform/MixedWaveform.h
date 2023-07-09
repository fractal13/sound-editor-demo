#ifndef _MIXEDWAVEFORM_H_
#define _MIXEDWAVEFORM_H_
#include "Waveform.h"
#include <vector>

class MixedWaveform:  public Waveform {
public:
  MixedWaveform(const std::string& name);
  virtual ~MixedWaveform();
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const;
  virtual MixedWaveform* clone() const;
  
  void addWaveform(Waveform *input);

protected:
  std::vector<Waveform *> mInputs;
private:
};

#endif /* _MIXEDWAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
