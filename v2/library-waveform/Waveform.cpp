#include "Waveform.h"
#include <cmath>

Waveform::Waveform(const std::string& name, const std::string& type_name) 
  : mName(name), mTypeName(type_name), mAmplitude(1.0), mFrequencyOffset(0.0) {
}

Waveform::~Waveform() {
}

const std::string& Waveform::getName() const {
  return mName;
}

const std::string& Waveform::getTypeName() const {
  return mTypeName;
}

double Waveform::getAmplitude() const {
  return mAmplitude;
}

double Waveform::getFrequencyOffset() const {
  return mFrequencyOffset;
}

void Waveform::setName(const std::string& name) {
  mName = name;
}

void Waveform::setTypeName(const std::string& type_name) {
  mTypeName = type_name;
}

void Waveform::setAmplitude(const double amplitude) {
  mAmplitude = amplitude;
}

void Waveform::setFrequencyOffset(const double frequency_offset) {
  mFrequencyOffset = frequency_offset;
}


double Waveform::computeSampleAngle(const double frequency, const double sample_number, const int samples_per_second) const {
  double frequency_scale = 1.0 + mFrequencyOffset;
  double angle = (two_pi * sample_number * frequency * frequency_scale) / samples_per_second;
  return angle;
}

double Waveform::computeSampleCyclePosition(const double frequency, const double sample_number, const int samples_per_second) const {
  double angle = computeSampleAngle(frequency, sample_number, samples_per_second);
  double cycle_position = std::fmod(angle, two_pi) / two_pi;
  return cycle_position;
}

void Waveform::generateSamples(const double frequency, const double seconds, const int samples_per_second, AudioTrack& track) const {
  track.setSize(samples_per_second, seconds);
  for (int n = 0; n < track.getSampleCount(); n++) {
    track[n] = generateOneSample(frequency, n, samples_per_second);
  }
}


