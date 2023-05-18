#include "StaffNote.h"
#include <exception>

StaffNote::StaffNote()
  : mNote(), mStart(0.0) {
}

StaffNote::StaffNote(const Note& note, const double start)
  : mNote(note), mStart(start) {
  if(start < 0.0) {
    throw std::exception();
  }
}
Note& StaffNote::getNote() {
  return mNote;
}

const Note& StaffNote::getNote() const {
  return mNote;
}

double StaffNote::getStart() const {
  return mStart;
}

void StaffNote::setStart(const double start) {
  if(start >= 0.0) {
    mStart = start;
  }
}
