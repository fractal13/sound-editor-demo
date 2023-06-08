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
    f = new SineWaveform(waveform);
  } else if(waveform == "sawtooth") {
    f = new SawtoothWaveform(waveform);
  } else if(waveform == "square") {
    f = new SquareWaveform(waveform);
  } else if(waveform == "triangle") {
    f = new TriangleWaveform(waveform);
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
    e = new ADSREnvelope(envelope, 0.50, 0.01, 0.01, 0.3, 0.01);
  } else if(envelope == "AD") {
    e = new ADEnvelope(envelope, 0.50, 0.01);
  } else if(envelope == "AR") {
    e = new AREnvelope(envelope, 0.50, 0.01, 0.3, 0.01);
  } else {
    std::stringstream ss;
    ss << "Invalid envelope name '" << envelope << "'";
    throw std::invalid_argument(ss.str());
  }
  Instrument::setEnvelope(e);
}
