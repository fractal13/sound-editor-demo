#include "Waveform.h"
#include <cmath>

Waveform::Waveform(const std::string& name, const std::string& type_name) 
  : mName(name), mTypeName(type_name) {
}

Waveform::~Waveform() {
}

const std::string& Waveform::getName() const {
  return mName;
}

const std::string& Waveform::getTypeName() const {
  return mTypeName;
}

void Waveform::setName(const std::string& name) {
  mName = name;
}

void Waveform::setTypeName(const std::string& type_name) {
  mTypeName = type_name;
}


double Waveform::computeSampleAngle(const double frequency, const double sample_number, const int samples_per_second) const {
  double angle = (two_pi * sample_number * frequency) / samples_per_second;
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


