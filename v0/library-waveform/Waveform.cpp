#include "Waveform.h"
#include <cmath>

Waveform::Waveform() {
}

Waveform::~Waveform() {
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

void Waveform::generateSamples(const double frequency, const double seconds, const int samples_per_second, std::vector<double>& samples) const {

  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double value = generateOneSample(frequency, n, samples_per_second);
    samples[n] = value;
  }

}


