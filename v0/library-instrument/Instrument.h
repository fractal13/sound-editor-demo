#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include "Waveform.h"
#include "Envelope.h"
#include <vector>
#include <string>

class Instrument {
public:
  Instrument();
  Instrument(const std::string& name, Waveform *waveform, Envelope *envelope);
  virtual ~Instrument();
  virtual Instrument *clone() const;

  const std::string& getName() const;
  Waveform *getWaveform() const;
  Envelope *getEnvelope() const;
  
  void setName(const std::string& name);
  void setWaveform(Waveform *waveform);
  void setEnvelope(Envelope *envelope);

  void generateSamples(const double frequency, const double seconds, const int samples_per_second, std::vector<double>& samples) const;
  
protected:
  std::string mName;
  Waveform *mWaveform;
  Envelope *mEnvelope;

private:
};

#endif /* _INSTRUMENT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
