#ifndef _STAFFNOTE_H_
#define _STAFFNOTE_H_

#include "Note.h"
#include <iostream>

class StaffNote {
public:
  StaffNote(); // defaults to default Note and 0.0 start
  StaffNote(const Note& note, const double start);
  Note& getNote();
  const Note& getNote() const;

  double getStart() const;
  void setStart(const double start);

private:
  Note   mNote;  // frequency and duration
  double mStart; // fraction of whole notes since beginning of staff
};

std::ostream& operator<<(std::ostream& os, const StaffNote& staff_note);

#endif /* _STAFFNOTE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
