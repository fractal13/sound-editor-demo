#ifndef _INSTRUMENTARIUM_H_
#define _INSTRUMENTARIUM_H_

#include "Instrument.h"
#include <map>
#include <string>

class Instrumentarium {
public:
  Instrumentarium();
  virtual ~Instrumentarium();
  void addInstrument(const std::string& name, Instrument *instrument);
  Instrument *getInstrument(const std::string& name);
protected:
  std::map<std::string, Instrument *> mInstruments;
private:
};

#endif /* _INSTRUMENTARIUM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
