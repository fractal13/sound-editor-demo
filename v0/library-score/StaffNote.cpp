#include "StaffNote.h"
#include <sstream>
#include <exception>
#include <stdexcept>

StaffNote::StaffNote()
  : mNote(), mStart(0.0) {
}

StaffNote::StaffNote(const Note& note, const double start)
  : mNote(note), mStart(start) {
  if(start < 0.0) {
    std::stringstream ss;
    ss << "StaffNote start must be non-negative.";
    throw std::invalid_argument(ss.str());
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
