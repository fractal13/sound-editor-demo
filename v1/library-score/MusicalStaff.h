#ifndef _MUSICALSTAFF_H_
#define _MUSICALSTAFF_H_

#include "Instrument.h"
#include "StaffNote.h"
#include "TimeSignature.h"
#include <vector>

class MusicalStaff {
public:
  MusicalStaff(); // defaults to no Instrument, and no notes
  MusicalStaff(Instrument *instrument); // defaults to no notes
  MusicalStaff(const MusicalStaff& src); // copy constructor
  ~MusicalStaff();

  const std::string& getName() const;
  void setName(const std::string& name);

  Instrument& getInstrument();
  const Instrument& getInstrument() const;
  void setInstrument(Instrument *instrument);

  void addNote(const StaffNote& note);
  const std::vector<StaffNote>& getNotes() const;

  double getDurationInWholeNotes() const;

  void render(const TimeSignature& time_signature, const double tempo, const int samples_per_second, std::vector<double>& values) const;

private:
  std::string             mName;
  Instrument             *mInstrument; // instrument to perform these notes
  std::vector<StaffNote>  mNotes;      // the notes to perform
};


#endif /* _MUSICALSTAFF_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
