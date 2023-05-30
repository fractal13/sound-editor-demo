#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_
#include <vector>

class Envelope {
public:
  Envelope();
  virtual ~Envelope();
  virtual void generateAmplitudes(const double seconds, const int samples_per_second, std::vector<double>& amplitudes) const = 0;
  virtual Envelope* clone() const = 0;
protected:
private:
};

#endif /* _ENVELOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
