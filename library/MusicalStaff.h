#ifndef _MUSICALSTAFF_H_
#define _MUSICALSTAFF_H_

#include "Instrument.h"
#include "StaffNote.h"
#include <vector>

class MusicalStaff {
public:
  MusicalStaff(); // defaults to no Instrument, and no notes
  MusicalStaff(Instrument *instrument); // defaults to no notes
  ~MusicalStaff();

  Instrument& getInstrument();
  const Instrument& getInstrument() const;
  void setInstrument(Instrument *instrument);

  void addNote(const StaffNote& note);
  const std::vector<StaffNote>& getNotes() const;

private:
  Instrument             *mInstrument; // instrument to perform these notes
  std::vector<StaffNote>  mNotes;      // the notes to perform
};



#endif /* _MUSICALSTAFF_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
