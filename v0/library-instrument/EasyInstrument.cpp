#include "EasyInstrument.h"

#include "SineWaveform.h"
#include "SawtoothWaveform.h"
#include "SquareWaveform.h"
#include "TriangleWaveform.h"

#include "ADSREnvelope.h"
#include "ADEnvelope.h"
#include "AREnvelope.h"

#include <sstream>
#include <stdexcept>

EasyInstrument::EasyInstrument()
  : Instrument() {
}

EasyInstrument::EasyInstrument(const std::string& waveform, const std::string& envelope) 
  : Instrument() {
  setWaveform(waveform);
  setEnvelope(envelope);
}

EasyInstrument::~EasyInstrument() {
}

void EasyInstrument::setWaveform(const std::string& waveform) {
  Waveform *f = 0;
  if(waveform == "sine") {
    f = new SineWaveform();
  } else if(waveform == "sawtooth") {
    f = new SawtoothWaveform();
  } else if(waveform == "square") {
    f = new SquareWaveform();
  } else if(waveform == "triangle") {
    f = new TriangleWaveform();
  } else {
    std::stringstream ss;
    ss << "Invalid waveform name '" << waveform << "'";
    throw std::invalid_argument(ss.str());
  }
  Instrument::setWaveform(f);
}

void EasyInstrument::setEnvelope(const std::string& envelope) {
  Envelope *e = 0;
  if(envelope == "ADSR") {
    e = new ADSREnvelope(0.01, 0.01, 0.5, 0.02);
  } else if(envelope == "AD") {
    e = new ADEnvelope(0.01);
  } else if(envelope == "AR") {
    e = new AREnvelope(0.01, 0.75, 0.02);
  } else {
    std::stringstream ss;
    ss << "Invalid envelope name '" << envelope << "'";
    throw std::invalid_argument(ss.str());
  }
  Instrument::setEnvelope(e);
}
