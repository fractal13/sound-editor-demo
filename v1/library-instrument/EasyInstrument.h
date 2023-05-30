#ifndef _EASYINSTRUMENT_H_
#define _EASYINSTRUMENT_H_

#include "Instrument.h"
#include <string>

class EasyInstrument: public Instrument {
public:
  EasyInstrument();
  EasyInstrument(const std::string& waveform, const std::string& envelope);
  ~EasyInstrument();

  void setWaveform(const std::string& waveform);
  void setEnvelope(const std::string& envelope);

  
protected:
private:
};

#endif /* _EASYINSTRUMENT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
