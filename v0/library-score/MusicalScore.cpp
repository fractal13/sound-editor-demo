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
  if(mTempo <= 0.0) {
    std::stringstream ss;
    ss << "Tempo must be positive.";
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
  staff.render(mTimeSignature, mTempo, samples_per_second, values);
}
