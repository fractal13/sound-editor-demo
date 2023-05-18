#ifndef _MUSICALSCORE_H_
#define _MUSICALSCORE_H_
#include "TimeSignature.h"
#include "MusicalStaff.h"

#include <vector>

class MusicalScore {
public:
  MusicalScore(); // defaults to 4/4 time, 100 beats/minute
  MusicalScore(const TimeSignature& time_signature, const double tempo);

  TimeSignature& getTimeSignature();
  const TimeSignature& getTimeSignature() const;
  double getTempo() const;
  void setTempo(const double tempo);

  unsigned int createStaff(); // adds a new MusicalStaff, returns its index
  MusicalStaff& getStaff(const unsigned int index); 
  const MusicalStaff& getStaff(const unsigned int index) const; 

private:
  TimeSignature mTimeSignature;      // 
  double mTempo;                     // Beats per minute
  std::vector<MusicalStaff> mStaves; // One staff per musical instrument
};

#endif /* _MUSICALSCORE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
