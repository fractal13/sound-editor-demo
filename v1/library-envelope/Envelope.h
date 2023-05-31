#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_
#include <vector>

class Envelope {
public:
  Envelope();
  Envelope(const double amplitude);
  virtual ~Envelope();
  virtual void generateAmplitudes(const double seconds, const int samples_per_second, std::vector<double>& amplitudes) const = 0;
  virtual Envelope* clone() const = 0;

  double getMaximumAmplitude() const;
  void setMaximumAmplitude(const double amplitude);
protected:
  double mMaximumAmplitude;
private:
};

#endif /* _ENVELOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
