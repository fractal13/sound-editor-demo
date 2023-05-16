#include "Note.h"
#include <map>
#include <string>

static const std::map<std::string, double> g_frequencies =
  {
   { "C3", 130.813 },
   { "D3", 146.832 },
   { "E3", 164.814 },
   { "F3", 174.614 },
   { "G3", 195.998 },
   { "A3", 220.000 },
   { "B3", 246.942 },
   { "C4", 261.626 },
   { "D4", 293.665 },
   { "E4", 329.628 },
   { "F4", 349.228 },
   { "G4", 391.995 },
   { "A4", 440.000 },
   { "B4", 493.883 },
  };

Note::Note(const std::string& name, const double& duration) 
  : mName(name), mDuration(duration) {
}

const std::string& Note::getName() const {
  return mName;
}

const double& Note::getDuration() const {
  return mDuration;
}

double Note::getFrequency() const {
  double frequency = 440.000;
  auto iter = g_frequencies.find(mName);
  if(iter != g_frequencies.end()) {
    frequency = iter->second;
  }
  return frequency;
}
