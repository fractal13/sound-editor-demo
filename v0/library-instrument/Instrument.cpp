#include "Instrument.h"

Instrument::Instrument() 
  : mName(""), mWaveform(0), mEnvelope(0) {
}
Instrument::Instrument(const std::string& name, Waveform *waveform, Envelope *envelope) 
  : mName(name), mWaveform(waveform), mEnvelope(envelope) {
}

Instrument::~Instrument() {
  if(mWaveform) {
    delete mWaveform;
    mWaveform = 0;
  }
  if(mEnvelope) {
    delete mEnvelope;
    mEnvelope = 0;
  }
}

Instrument *Instrument::clone() const {
  Waveform *waveform = 0;
  if(mWaveform) {
    waveform = mWaveform->clone();
  }
  Envelope *envelope = 0;
  if(mEnvelope) {
    envelope = mEnvelope->clone();
  }

  auto copy = new Instrument(mName, waveform, envelope);
  return copy;
}


const std::string& Instrument::getName() const {
  return mName;
}

Waveform *Instrument::getWaveform() const {
  return mWaveform;
}
Envelope *Instrument::getEnvelope() const {
  return mEnvelope;
}

void Instrument::setName(const std::string& name) {
  mName = name;
}

void Instrument::setWaveform(Waveform *waveform) {
  if(mWaveform) {
    delete mWaveform;
    mWaveform = 0;
  }
  mWaveform = waveform;
}

void Instrument::setEnvelope(Envelope *envelope) {
  if(mEnvelope) {
    delete mEnvelope;
    mEnvelope = 0;
  }
  mEnvelope = envelope;
}

void Instrument::generateSamples(const double frequency, const double seconds, const int samples_per_second, std::vector<double>& samples) const {
  std::vector<double> amplitudes;
  mEnvelope->generateAmplitudes(seconds, samples_per_second, amplitudes);
  std::vector<double> raw_samples;
  mWaveform->generateSamples(frequency, seconds, samples_per_second, raw_samples);
  // FIXME: With a pair of classes, can use operator overloading here.
  unsigned int N = samples_per_second * seconds; // number of samples in note
  samples.resize(N);
  unsigned int n;
  for(n = 0; n < N; n++) {
    samples[n] = amplitudes[n] * raw_samples[n];
  }
}
