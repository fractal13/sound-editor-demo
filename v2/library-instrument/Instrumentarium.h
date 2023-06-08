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
  Instrument *getInstrument(const int midi_instrument_number);

  typedef std::map<std::string, Instrument *>::iterator iterator;
  typedef std::map<std::string, Instrument *>::const_iterator const_iterator;
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

protected:
  std::map<std::string, Instrument *> mInstruments;
private:
};

#endif /* _INSTRUMENTARIUM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
