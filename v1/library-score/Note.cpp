#include "Note.h"
#include <map>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>

// FIXME all frequencies should be computed.
static const std::map<std::string, double> g_frequencies =
  {
   { "C2", 65.406 },
   { "C#2", 69.296 },
   { "Db2", 69.296 },
   { "D2", 73.416 },
   { "D#2", 77.782 },
   { "Eb2", 77.782 },
   { "E2", 82.407 },
   { "F2", 87.307 },
   { "F#2", 92.499 },
   { "Gb2", 92.499 },
   { "G2", 97.999 },
   { "G#2", 103.826 },
   { "Ab2", 103.826 },
   { "A2", 110.000 },
   { "A#2", 116.541 },
   { "Bb2", 116.541 },
   { "B2", 123.471 },
   
   { "C3", 130.813 },
   { "C#3", 138.591 },
   { "Db3", 138.591 },
   { "D3", 146.832 },
   { "D#3", 155.563 },
   { "Eb3", 155.563 },
   { "E3", 164.814 },
   { "F3", 174.614 },
   { "F#3", 184.997 },
   { "Gb3", 184.997 },
   { "G3", 195.998 },
   { "G#3", 207.652 },
   { "Ab3", 207.652 },
   { "A3", 220.000 },
   { "A#3", 233.082 },
   { "Bb3", 233.082 },
   { "B3", 246.942 },
   
   { "C4", 261.626 },
   { "C#4", 277.183 },
   { "Db4", 277.183 },
   { "D4", 293.665 },
   { "D#4", 311.127 },
   { "Eb4", 311.127 },
   { "E4", 329.628 },
   { "F4", 349.228 },
   { "F#4", 369.994 },
   { "Gb4", 369.994 },
   { "G4", 391.995 },
   { "G#4", 415.305 },
   { "Ab4", 415.305 },
   { "A4", 440.000 },
   { "A#4", 466.164 },
   { "Bb4", 466.164 },
   { "B4", 493.883 },

   { "C5", 523.251 },
   { "C#5", 554.365 },
   { "Db5", 554.365 },
   { "D5", 587.330 },
   { "D#5", 622.254 },
   { "Eb5", 622.254 },
   { "E5", 659.255 },
   { "F5", 698.456 },
   { "F#5", 739.989 },
   { "Gb5", 739.989 },
   { "G5", 783.991 },
   { "G#5", 830.609 },
   { "Ab5", 830.609 },
   { "A5", 880.000 },
   { "A#5", 932.328 },
   { "Bb5", 932.328 },
   { "B5", 987.767 },
  };

Note::Note() 
  : mName(""), mDuration(0.0) {
}

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
  } else {
    std::stringstream ss;
    ss << "Unknown note: " << mName << ".";
    throw std::invalid_argument(ss.str());
  }
  return frequency;
}

void Note::setName(const std::string& name) {
  auto iter = g_frequencies.find(name);
  if(iter != g_frequencies.end()) {
    mName = name;
  }
}
void Note::setDuration(const double duration) {
  if(duration > 0.0) {
    mDuration = duration;
  }
}
