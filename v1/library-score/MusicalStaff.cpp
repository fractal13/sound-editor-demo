#include "MusicalStaff.h"
#include "TimeSignature.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

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

const std::string& MusicalStaff::getName() const {
  return mName;
}

void MusicalStaff::setName(const std::string& name) {
  mName = name;
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

void MusicalStaff::render(const TimeSignature& time_signature, const double tempo, const int samples_per_second, AudioTrack& staff_track) const {
  double duration     = getDurationInWholeNotes();
  double beat_count   = duration * time_signature.getBeatValue();
  double second_count = beat_count * 60.0 / tempo;
  staff_track.setSize(samples_per_second, second_count);

  double whole_note_seconds = time_signature.getBeatValue() * 60.0 / tempo;
  
  AudioTrack instrument_track;
  for(auto& note : mNotes) {
    double start = note.getStart();
    double frequency = note.getNote().getFrequency();
    double seconds = note.getNote().getDuration() * whole_note_seconds;
    mInstrument->generateSamples(frequency, seconds, samples_per_second, instrument_track);
    instrument_track.setOffsetSeconds(start*whole_note_seconds);

    staff_track += instrument_track;
  }

}
