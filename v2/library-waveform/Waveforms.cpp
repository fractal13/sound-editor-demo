#include "Waveforms.h"

Waveforms::Waveforms() {
}

Waveforms::~Waveforms() {
  for(auto& item: mWaveforms) {
    if(item.second) {
      delete item.second;
      item.second = 0;
    }
  }
}

void Waveforms::addWaveform(const std::string& name, Waveform *waveform) {
  auto item = mWaveforms.find(name);
  if(item != mWaveforms.end()) {
    if(item->second) {
      delete item->second;
      item->second = 0;
    }
  }
  mWaveforms[name] = waveform;
}

Waveform *Waveforms::getWaveform(const std::string& name) {
  Waveform *waveform = 0;
  auto item = mWaveforms.find(name);
  if(item != mWaveforms.end()) {
    waveform = item->second->clone();
  }
  return waveform;
}
