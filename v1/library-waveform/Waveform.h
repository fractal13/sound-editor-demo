#ifndef _WAVEFORM_H_
#define _WAVEFORM_H_

#include "AudioTrack.h"

#include <vector>
#include <cmath>

//constexpr double two_pi = 6.283185307179586476925286766559;
constexpr double two_pi = 8.0*std::atan(1);

class Waveform {
public:
  Waveform();
  virtual ~Waveform();
  double computeSampleAngle(const double frequency, const double sample_number, const int samples_per_second) const;
  double computeSampleCyclePosition(const double frequency, const double sample_number, const int samples_per_second) const;

  virtual void generateSamples(const double frequency, const double seconds, const int samples_per_second, AudioTrack& track) const;
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const = 0;
  virtual Waveform* clone() const = 0;
protected:
private:
};


#endif /* _WAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
