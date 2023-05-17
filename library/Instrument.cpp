#include "Instrument.h"
#include <vector>
#include <cmath>


Instrument::Instrument() {
}

void Instrument::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {
  
  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double value1 = std::sin((two_pi * n * frequency) / samples_per_second);
    samples[n] = value1;
  }
}

Instrument2::Instrument2() 
  : Instrument() {
}

void Instrument2::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {

  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double value1 = std::cos((two_pi * n * frequency) / samples_per_second);
    if(value1 > 0.5) {
      value1 = 1.0;
    } else if(value1 < -0.5) {
      value1 = -1.0;
    }
    samples[n] = value1;
  }
}

Instrument3::Instrument3() 
  : Instrument() {
}

void Instrument3::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {

  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double angle = (two_pi * n * frequency) / samples_per_second;
    double position = std::fmod(angle, two_pi)/two_pi;
    double value = 0.0;
    if(position < 0.25) {
      double m = (1.0/0.25);
      double b = 0.0;
      double x = (position - 0.0);
      value = m*x+b;
    } else if(position < 0.75) {
      double m = (-2.0/0.5);
      double b = 1.0;
      double x = (position - 0.25);
      value = m*x+b;
    } else {
      double m = (1.0/0.25);
      double b = -1.0;
      double x = (position - 0.75);
      value = m*x+b;
    }
    samples[n] = value;
  }
}

Instrument4::Instrument4() 
  : Instrument() {
}

void Instrument4::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {

  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double angle = (two_pi * n * frequency) / samples_per_second;
    double position = std::fmod(angle, two_pi)/two_pi;
    double value = 0.0;
    if(position < 0.5) {
      value = 1.0;
    } else {
      value = -1.0;
    }
    samples[n] = value;
  }
}
