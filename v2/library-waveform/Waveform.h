#ifndef _WAVEFORM_H_
#define _WAVEFORM_H_

#include "AudioTrack.h"

#include <vector>
#include <cmath>
#include <string>

//constexpr double two_pi = 6.283185307179586476925286766559;
constexpr double two_pi = 8.0*std::atan(1);

class Waveform {
public:
  Waveform(const std::string& name, const std::string& type_name);
  virtual ~Waveform();

  const std::string& getName() const;
  const std::string& getTypeName() const;
  double getAmplitude() const;
  double getFrequencyOffset() const;

  void setName(const std::string& name);
  void setTypeName(const std::string& type_name);
  void setAmplitude(const double amplitude);
  void setFrequencyOffset(const double frequency_offset);

  double computeSampleAngle(const double frequency, const double sample_number, const int samples_per_second) const;
  double computeSampleCyclePosition(const double frequency, const double sample_number, const int samples_per_second) const;

  virtual void generateSamples(const double frequency, const double seconds, const int samples_per_second, AudioTrack& track) const;
  virtual double generateOneSample(const double frequency, const int sample_number, const double samples_per_second) const = 0;
  virtual Waveform* clone() const = 0;

protected:
  std::string mName;
  std::string mTypeName;
  double mAmplitude;
  double mFrequencyOffset;
private:
};


#endif /* _WAVEFORM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
