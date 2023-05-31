#include "AudioTrack.h"

#include <sstream>
#include <stdexcept>

AudioTrack::AudioTrack() 
  : mSamplesPerSecond(0), mOffsetSeconds(0.0), mSeconds(0), mValues(0) {
}

AudioTrack::~AudioTrack() {
}

bool AudioTrack::indexValid(const unsigned int i) const {
  return i < mValues.size();
}

int AudioTrack::getSamplesPerSecond() const {
  return mSamplesPerSecond;
}

double AudioTrack::getOffsetSeconds() const {
  return mOffsetSeconds;
}

double AudioTrack::getSeconds() const {
  return mSeconds;
}

int AudioTrack::getSampleCount() const {
  return mValues.size();
}

unsigned int AudioTrack::size() const {
  return mValues.size();
}

unsigned int AudioTrack::getOffset() const {
  return mSamplesPerSecond * mOffsetSeconds;
}

void AudioTrack::setSize(const int samples_per_second, const double seconds) {
  if(samples_per_second > 0 && seconds > 0.0) {
    mSamplesPerSecond = samples_per_second;
    mSeconds = seconds;
    resizeValues();
  }
}

void AudioTrack::setSamplesPerSecond(const int samples_per_second) {
  if(samples_per_second > 0) {
    mSamplesPerSecond = samples_per_second;
    resizeValues();
  }
}

void AudioTrack::setOffsetSeconds(const double offset_seconds) {
  if(offset_seconds > 0) {
    mOffsetSeconds = offset_seconds;
  }
}

void AudioTrack::setSeconds(const double seconds) {
  if(seconds > 0) {
    mSeconds = seconds;
    resizeValues();
  }
}

const double& AudioTrack::operator[](const unsigned int i) const {
  if(indexValid(i)) {
    return mValues[i];
  } else {
    static double error = -1.0;
    static double error_copy = -1.0;
    error_copy = error;
    return error_copy;
  }
}

double& AudioTrack::operator[](const unsigned int i) {
  if(indexValid(i)) {
    return mValues[i];
  } else {
    static double error = -1.0;
    static double error_copy = -1.0;
    error_copy = error;
    return error_copy;
  }
}

AudioTrack AudioTrack::operator*(const AudioTrack& rhs) const {
  AudioTrack track;
  if((mSamplesPerSecond != rhs.mSamplesPerSecond) ||
     (mSeconds != rhs.mSeconds)) {
    std::stringstream ss;
    ss << "AudioTrack objects must be same size to multiply." << std::endl;
    ss << "lhs.mSamplesPerSecond: " << mSamplesPerSecond << std::endl;
    ss << "lhs.mSeconds: " << mSeconds << std::endl;
    ss << "rhs.mSamplesPerSecond: " << rhs.mSamplesPerSecond << std::endl;
    ss << "rhs.mSeconds: " << rhs.mSeconds << std::endl;
    throw std::invalid_argument(ss.str());
  }

  track.setSize(mSamplesPerSecond, mSeconds);
  unsigned int i;
  for(i = 0; i < mValues.size(); i++) {
    track[i] = (*this)[i] * rhs[i];
  }
  return track;
}

AudioTrack& AudioTrack::operator+=(const AudioTrack& rhs) {
  unsigned int loffset = getOffset();
  unsigned int offset = rhs.getOffset();
  if((loffset > offset) ||
     (offset + rhs.size() > loffset + size())) {
    std::stringstream ss;
    ss << "AudioTrack lhs += rhs must have rhs fit entirely inside lhs." << std::endl;
    ss << "lhs.mSamplesPerSecond: " << mSamplesPerSecond << std::endl;
    ss << "lhs.mOffsetSeconds: " << mOffsetSeconds << std::endl;
    ss << "lhs.mSeconds: " << mSeconds << std::endl;
    ss << "rhs.mSamplesPerSecond: " << rhs.mSamplesPerSecond << std::endl;
    ss << "rhs.mOffsetSeconds: " << rhs.mOffsetSeconds << std::endl;
    ss << "rhs.mSeconds: " << rhs.mSeconds << std::endl;
    throw std::invalid_argument(ss.str());
  }

  unsigned int i;
  for(i = 0; i < rhs.size(); i++) {
    (*this)[offset-loffset+i] += rhs[i];
  }

  return *this;
}

void AudioTrack::resizeValues() {
  int N = mSamplesPerSecond * mSeconds;
  mValues.resize(0);
  mValues.resize(N, 0.0);
}
