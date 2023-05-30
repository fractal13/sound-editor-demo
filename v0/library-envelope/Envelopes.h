#ifndef _ENVELOPES_H_
#define _ENVELOPES_H_

#include "Envelope.h"
#include <map>
#include <string>

class Envelopes {
public:
  Envelopes();
  virtual ~Envelopes();
  void addEnvelope(const std::string& name, Envelope *envelope);
  Envelope *getEnvelope(const std::string& name);
protected:
  std::map<std::string, Envelope *> mEnvelopes;
private:
};

#endif /* _ENVELOPES_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
