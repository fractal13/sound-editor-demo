#include "Instrumentarium.h"

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
    instrument = item->second;
  }
  return instrument;
}
