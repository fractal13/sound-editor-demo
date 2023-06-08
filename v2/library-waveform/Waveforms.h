#ifndef _WAVEFORMS_H_
#define _WAVEFORMS_H_

#include "Waveform.h"
#include <map>
#include <string>

class Waveforms {
public:
  Waveforms();
  virtual ~Waveforms();
  void addWaveform(const std::string& name, Waveform *waveform);
  Waveform *getWaveform(const std::string& name);
protected:
  std::map<std::string, Waveform *> mWaveforms;
private:
};

#endif /* _WAVEFORMS_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
