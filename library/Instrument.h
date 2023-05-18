#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_
#include <vector>

constexpr double two_pi = 6.283185307179586476925286766559;

class Instrument {
public:
  Instrument();
  virtual ~Instrument();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
  virtual Instrument* clone() const;
private:
};

class Instrument2: public Instrument {
public:
  Instrument2();
  virtual ~Instrument2();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
  virtual Instrument* clone() const;
private:
};

class Instrument3: public Instrument {
public:
  Instrument3();
  virtual ~Instrument3();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
  virtual Instrument* clone() const;
private:
};

class Instrument4: public Instrument {
public:
  Instrument4();
  virtual ~Instrument4();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
  virtual Instrument* clone() const;
private:
};



#endif /* _INSTRUMENT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
