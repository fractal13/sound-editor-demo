#include "MusicalScore.h"
#include "Envelope.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

MusicalScore::MusicalScore()
  : mTimeSignature(), mTempo(100.0), mStaves() {
}

MusicalScore::MusicalScore(const TimeSignature& time_signature, const double tempo)
  : mTimeSignature(time_signature), mTempo(tempo), mStaves() {
  if(mTempo < 0.0) {
    std::stringstream ss;
    ss << "Tempo must be non-negative.";
    throw std::invalid_argument(ss.str());
  }
}


TimeSignature& MusicalScore::getTimeSignature() {
  return mTimeSignature;
}

const TimeSignature& MusicalScore::getTimeSignature() const {
  return mTimeSignature;
}

double MusicalScore::getTempo() const {
  return mTempo;
}

void MusicalScore::setTempo(const double tempo) {
  if(tempo > 0.0) {
    mTempo = tempo;
  }
}

unsigned int MusicalScore::createStaff() {
  MusicalStaff dummy;
  mStaves.push_back(dummy);
  return mStaves.size() - 1;
}

MusicalStaff& MusicalScore::getStaff(const unsigned int index) {
  if(index < mStaves.size()) {
    return mStaves[index];
  } else {
    static MusicalStaff error;
    return error;
  }
}

const MusicalStaff& MusicalScore::getStaff(const unsigned int index) const {
  if(index < mStaves.size()) {
    return mStaves[index];
  } else {
    static MusicalStaff error;
    return error;
  }
}

unsigned int MusicalScore::getNumberOfStaves() const {
  return mStaves.size();
}

void MusicalScore::renderStaff(const unsigned int index, const int samples_per_second, std::vector<double>& values) const {
  const MusicalStaff& staff = getStaff(index);
  double duration = staff.getDurationInWholeNotes();
  double beat_count = duration * mTimeSignature.getBeatValue();
  double second_count = beat_count * 60.0 / mTempo;
  unsigned int sample_count = second_count * samples_per_second;
  values.resize(0);
  values.resize(sample_count, 0.0);

  // FIXME
  double max_amplitude   = 1.00;  // "volume"
  double attack_seconds  = 0.10;
  double decay_seconds   = 0.05;
  double release_seconds = 0.10;
  double sustain_amplitude = max_amplitude * 0.5;
  Envelope envelope(max_amplitude, attack_seconds, decay_seconds, sustain_amplitude, release_seconds);

  const Instrument& instrument = staff.getInstrument();

  double whole_note_seconds = mTimeSignature.getBeatValue() * 60.0 / mTempo;
  for(auto& note : staff.getNotes()) {
    double start = note.getStart();
    double frequency = note.getNote().getFrequency();
    double seconds = note.getNote().getDuration() * whole_note_seconds;

    std::vector<double> amplitudes;
    envelope.generateAmplitudes(amplitudes, seconds, samples_per_second);
    std::vector<double> samples;
    instrument.generateSamples(samples, frequency, seconds, samples_per_second);

    unsigned int note_start = samples_per_second * start * whole_note_seconds;
    unsigned int N = samples_per_second * seconds; // number of samples in note
    for (unsigned int n = 0; n < N; n++) {
      if(note_start + n >= values.size()) {
        std::stringstream ss;
        ss << "Sample position is beyond the end of the space.";
        throw std::invalid_argument(ss.str());
      } else {
        values[note_start + n] = amplitudes[n] * samples[n];
      }
    }
  }

}
