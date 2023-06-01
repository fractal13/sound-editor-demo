#include "Envelope.h"

Envelope::Envelope(const std::string& name, const std::string& type_name) 
  : mName(name), mTypeName(type_name), mMaximumAmplitude(1.0) {
}

Envelope::Envelope(const std::string& name, const std::string& type_name, const double amplitude) 
  : mName(name), mTypeName(type_name), mMaximumAmplitude(amplitude) {
}

Envelope::~Envelope() {
}

const std::string& Envelope::getName() const {
  return mName;
}

const std::string& Envelope::getTypeName() const {
  return mTypeName;
}

double Envelope::getMaximumAmplitude() const {
  return mMaximumAmplitude;
}

void Envelope::setName(const std::string& name) {
  mName = name;
}

void Envelope::setTypeName(const std::string& type_name) {
  mTypeName = type_name;
}

void Envelope::setMaximumAmplitude(const double amplitude) {
  if(amplitude > 0.0) {
    mMaximumAmplitude = amplitude;
  }
}


