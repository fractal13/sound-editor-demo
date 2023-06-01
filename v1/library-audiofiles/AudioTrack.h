#ifndef _AUDIOTRACK_H_
#define _AUDIOTRACK_H_

#include <vector>

class AudioTrack {
public:
  AudioTrack();
  virtual ~AudioTrack();

  bool indexValid(const unsigned int i) const;

  int getSamplesPerSecond() const;
  double getOffsetSeconds() const;
  double getSeconds() const;
  int getSampleCount() const;
  unsigned int size() const;
  unsigned int getOffset() const;

  void setSize(const int samples_per_second, const double seconds);
  void setSamplesPerSecond(const int samples_per_second);
  void setOffsetSeconds(const double offset_seconds);
  void setSeconds(const double seconds);

  const double& operator[](const unsigned int i) const;
  double& operator[](const unsigned int i);

  AudioTrack operator*(const AudioTrack& rhs) const;
  AudioTrack operator*(const double rhs) const;

  AudioTrack& operator+=(const AudioTrack& rhs);
  
protected:
  void resizeValues();

  int mSamplesPerSecond;
  double mOffsetSeconds;
  double mSeconds;
  std::vector<double> mValues;

private:
};

#endif /* _AUDIOTRACK_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
