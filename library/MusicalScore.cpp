#include "MusicalScore.h"
#include <exception>

MusicalScore::MusicalScore()
  : mTimeSignature(), mTempo(100.0), mStaves() {
}

MusicalScore::MusicalScore(const TimeSignature& time_signature, const double tempo)
  : mTimeSignature(time_signature), mTempo(tempo), mStaves() {
  if(mTempo < 0.0) {
    throw std::exception();
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
  mStaves.push_back(MusicalStaff());
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
