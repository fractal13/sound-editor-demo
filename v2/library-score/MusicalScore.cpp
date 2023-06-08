#include "MusicalScore.h"
#include "Envelope.h"
#include "debug.h"
#include <iostream>
#include <set>

MusicalScore::MusicalScore()
  : mTimeSignature(), mTempo(100.0), mStaves() {
}

MusicalScore::MusicalScore(const TimeSignature& time_signature, const double tempo)
  : mTimeSignature(time_signature), mTempo(tempo), mStaves() {
  if(mTempo <= 0.0) {
    DEBUG_INVALID("Tempo must be positive. '" << mTempo << "' is not positive.");
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

void MusicalScore::setUniqueStaffName(MusicalStaff& staff) const {
  std::set<std::string> names;
  for(const auto& s: mStaves) {
    names.insert(s.getName());
  }
  int count = 0;
  std::stringstream ss;
  while(true) {
    ss.str("");
    ss << "unique-staff-" << count;
    if(names.count(ss.str()) == 0) {
      staff.setName(ss.str());
      break;
    }
    count++;
  }
}

Instrumentarium& MusicalScore::getInstrumentarium() {
  return mInstrumentarium;
}

const Instrumentarium& MusicalScore::getInstrumentarium() const {
  return mInstrumentarium;
}

Waveforms& MusicalScore::getWaveforms() {
  return mWaveforms;
}

const Waveforms& MusicalScore::getWaveforms() const {
  return mWaveforms;
}

Envelopes& MusicalScore::getEnvelopes() {
  return mEnvelopes;
}

const Envelopes& MusicalScore::getEnvelopes() const {
  return mEnvelopes;
}

Mixer& MusicalScore::getMixer() {
  return mMixer;
}

const Mixer& MusicalScore::getMixer() const {
  return mMixer;
}

void MusicalScore::renderStaff(const unsigned int index, const int samples_per_second, AudioTrack& track) const {
  const MusicalStaff& staff = getStaff(index);
  staff.render(mTimeSignature, mTempo, samples_per_second, track);
}
