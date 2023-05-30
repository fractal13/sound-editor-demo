#include "Envelopes.h"

Envelopes::Envelopes() {
}

Envelopes::~Envelopes() {
  for(auto& item: mEnvelopes) {
    if(item.second) {
      delete item.second;
      item.second = 0;
    }
  }
}

void Envelopes::addEnvelope(const std::string& name, Envelope *envelope) {
  auto item = mEnvelopes.find(name);
  if(item != mEnvelopes.end()) {
    if(item->second) {
      delete item->second;
      item->second = 0;
    }
  }
  mEnvelopes[name] = envelope;
}

Envelope *Envelopes::getEnvelope(const std::string& name) {
  Envelope *envelope = 0;
  auto item = mEnvelopes.find(name);
  if(item != mEnvelopes.end()) {
    envelope = item->second->clone();
  }
  return envelope;
}
