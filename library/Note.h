#ifndef _NOTE_H_
#define _NOTE_H_
#include <string>

const double QUARTER_NOTE = 0.25;
const double HALF_NOTE = 0.50;
const double WHOLE_NOTE = 1.00;

class Note {
public:
  Note(const std::string& name, const double& duration);
  const std::string& getName() const;
  const double& getDuration() const;
  double getFrequency() const;
private:
  std::string mName; // E4, C3, etc.
  double mDuration;  // fraction of a whole note; quarter note = 0.25
};


#endif /* _NOTE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
