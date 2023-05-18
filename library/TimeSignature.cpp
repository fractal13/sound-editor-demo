#include "TimeSignature.h"
#include <exception>

TimeSignature::TimeSignature()
  : mBeatsPerBar(4), mBeatValue(4) {
}

TimeSignature::TimeSignature(const int beats_per_bar, const int beat_value)
  : mBeatsPerBar(beats_per_bar), mBeatValue(beat_value) {
  if((mBeatsPerBar <= 0) || (mBeatValue <= 0)) {
    throw std::exception();
  }
}

int TimeSignature::getBeatsPerBar() const {
  return mBeatsPerBar;
}

int TimeSignature::getBeatValue() const {
  return mBeatValue;
}

void TimeSignature::setBeatsPerBar(const int beats_per_bar) {
  if(beats_per_bar > 0) {
    mBeatsPerBar = beats_per_bar;
  }
}

void TimeSignature::setBeatValue(const int beat_value) {
  if(beat_value > 0) {
    mBeatValue = beat_value;
  }
}
