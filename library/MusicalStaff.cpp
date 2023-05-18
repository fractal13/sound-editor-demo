#include "MusicalStaff.h"

MusicalStaff::MusicalStaff()
  : mInstrument(0), mNotes() {
}

MusicalStaff::MusicalStaff(Instrument *instrument)
  : mInstrument(instrument), mNotes() {
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
  }
  mInstrument = instrument;
}

void MusicalStaff::addNote(const StaffNote& note) {
  mNotes.push_back(note);
}

const std::vector<StaffNote>& MusicalStaff::getNotes() const {
  return mNotes;
}
