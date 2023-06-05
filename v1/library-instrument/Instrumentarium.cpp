#include "Instrumentarium.h"
#include "EasyInstrument.h"
#include <sstream>

Instrumentarium::Instrumentarium() {
}

Instrumentarium::~Instrumentarium() {
  for(auto& item: mInstruments) {
    if(item.second) {
      delete item.second;
      item.second = 0;
    }
  }
}

void Instrumentarium::addInstrument(const std::string& name, Instrument *instrument) {
  auto item = mInstruments.find(name);
  if(item != mInstruments.end()) {
    if(item->second) {
      delete item->second;
      item->second = 0;
    }
  }
  mInstruments[name] = instrument;
}

Instrument *Instrumentarium::getInstrument(const std::string& name) {
  Instrument *instrument = 0;
  auto item = mInstruments.find(name);
  if(item != mInstruments.end()) {
    instrument = item->second->clone();
  }
  return instrument;
}

Instrument *Instrumentarium::getInstrument(const int midi_instrument_number) {
  std::stringstream ss;
  ss << "midi-instrument-" << midi_instrument_number;
  std::string name = ss.str();
  auto item = mInstruments.find(name);
  if(item == mInstruments.end()) {
    // FIXME: choose instrument based on actual midi number
    Instrument *midi_instrument = new EasyInstrument("sine", "ADSR");
    midi_instrument->setName(name);
    addInstrument(name, midi_instrument);
  }
  return getInstrument(name);
}

Instrumentarium::iterator Instrumentarium::begin() { 
  return mInstruments.begin(); 
}

Instrumentarium::const_iterator Instrumentarium::begin() const { 
  return mInstruments.begin(); 
}

Instrumentarium::iterator Instrumentarium::end() { 
  return mInstruments.end(); 
}

Instrumentarium::const_iterator Instrumentarium::end() const {
  return mInstruments.end();
}
