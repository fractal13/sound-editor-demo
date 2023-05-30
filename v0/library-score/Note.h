#ifndef _NOTE_H_
#define _NOTE_H_
#include <string>

constexpr double EIGHTH_NOTE = 0.125;
constexpr double QUARTER_NOTE = 0.25;
constexpr double HALF_NOTE = 0.50;
constexpr double WHOLE_NOTE = 1.00;

class Note {
public:
  Note();
  Note(const std::string& name, const double& duration);
  const std::string& getName() const;
  const double& getDuration() const;
  double getFrequency() const;

  void setName(const std::string& name);
  void setDuration(const double duration);
private:
  std::string mName; // E4, C3, etc.
  double mDuration;  // fraction of a whole note; quarter note = 0.25
};


#endif /* _NOTE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
