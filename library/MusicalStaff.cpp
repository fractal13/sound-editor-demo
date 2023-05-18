#include "MusicalStaff.h"
#include <iostream>

MusicalStaff::MusicalStaff()
  : mInstrument(0), mNotes() {
}

MusicalStaff::MusicalStaff(Instrument *instrument)
  : mInstrument(instrument), mNotes() {
}

MusicalStaff::MusicalStaff(const MusicalStaff& src)
  : mInstrument(0), mNotes(src.mNotes) {
  if(src.mInstrument) {
    mInstrument = src.mInstrument->clone();
  }
}

MusicalStaff::~MusicalStaff() {
  if(mInstrument) {
    delete mInstrument;
    mInstrument = 0;
  }
}

Instrument& MusicalStaff::getInstrument() {
  return *mInstrument;
}

const Instrument& MusicalStaff::getInstrument() const {
  return *mInstrument;
}

void MusicalStaff::setInstrument(Instrument *instrument) {
  if(mInstrument) {
    delete mInstrument;
    mInstrument = 0;
  }
  mInstrument = instrument;
}

void MusicalStaff::addNote(const StaffNote& note) {
  mNotes.push_back(note);
}

const std::vector<StaffNote>& MusicalStaff::getNotes() const {
  return mNotes;
}

double MusicalStaff::getDurationInWholeNotes() const {
  double duration = 0.0;
  for(auto& note : mNotes) {
    double end = note.getStart() + note.getNote().getDuration();
    if(end > duration) {
      duration = end;
    }
  }
  return duration;
}
