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

void Instrument::generateSamples(const double frequency, const double seconds, const int samples_per_second, AudioTrack& track) const {
  AudioTrack envelope;
  AudioTrack waveform;
  mEnvelope->generateAmplitudes(seconds, samples_per_second, envelope);
  mWaveform->generateSamples(frequency, seconds, samples_per_second, waveform);
  
  track = envelope * waveform;
}
