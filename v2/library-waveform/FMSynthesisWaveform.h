#ifndef _FMSYNTHESISWAVEFORM_H_
#define _FMSYNTHESISWAVEFORM_H_

#include "Waveform.h"
class FMSynthesisWaveform: public Waveform {
public:
  FMSynthesisWaveform(const std::string& name);
  virtual ~FMSynthesisWaveform();
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const;
  virtual FMSynthesisWaveform* clone() const;
  double getBeta() const;
  double getFrequencyRatio() const;
  void setBeta(const double beta);
  void setFrequencyRatio(const double frequency_ratio);
protected:
  double mBeta;
  double mFrequencyRatio;
private:
};

#endif /* _FMSYNTHESISWAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
