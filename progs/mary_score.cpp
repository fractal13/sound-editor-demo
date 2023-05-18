#include "MusicalScore.h"
#include "WaveFile.h"
#include <vector>
#include <sstream>

void read_score(MusicalScore& score) {
  score.getTimeSignature().setBeatsPerBar(4);
  score.getTimeSignature().setBeatValue(4);
  score.setTempo(120);

  unsigned int treble_index = score.createStaff();
  MusicalStaff& treble = score.getStaff(treble_index);
  treble.setInstrument(new Instrument);
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 0.00));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 0.25));
  treble.addNote(StaffNote(Note("C4", QUARTER_NOTE), 0.50));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 0.75));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 1.00));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 1.25));
  treble.addNote(StaffNote(Note("E4", HALF_NOTE),    1.50));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 2.00));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 2.25));
  treble.addNote(StaffNote(Note("D4", HALF_NOTE),    2.50));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 3.00));
  treble.addNote(StaffNote(Note("G4", QUARTER_NOTE), 3.25));
  treble.addNote(StaffNote(Note("G4", HALF_NOTE),    3.50));

  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 4.00));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 4.25));
  treble.addNote(StaffNote(Note("C4", QUARTER_NOTE), 4.50));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 4.75));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 5.00));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 5.25));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 5.50));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 5.75));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 6.00));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 6.25));
  treble.addNote(StaffNote(Note("E4", QUARTER_NOTE), 6.50));
  treble.addNote(StaffNote(Note("D4", QUARTER_NOTE), 6.75));
  treble.addNote(StaffNote(Note("C4", WHOLE_NOTE),   7.00));

  unsigned int base_index = score.createStaff();
  MusicalStaff& base = score.getStaff(base_index);
  base.setInstrument(new Instrument);
  base.addNote(StaffNote(Note("C3", HALF_NOTE),      0.00));
  base.addNote(StaffNote(Note("G3", HALF_NOTE),      0.50));
  base.addNote(StaffNote(Note("C3", HALF_NOTE),      1.00));
  base.addNote(StaffNote(Note("G3", HALF_NOTE),      1.50));
  base.addNote(StaffNote(Note("G3", HALF_NOTE),      2.00));
  base.addNote(StaffNote(Note("B3", HALF_NOTE),      2.50));
  base.addNote(StaffNote(Note("C3", HALF_NOTE),      3.00));
  base.addNote(StaffNote(Note("G3", HALF_NOTE),      3.50));

  base.addNote(StaffNote(Note("C3", HALF_NOTE),      4.00));
  base.addNote(StaffNote(Note("G3", HALF_NOTE),      4.50));
  base.addNote(StaffNote(Note("C3", HALF_NOTE),      5.00));
  base.addNote(StaffNote(Note("G3", HALF_NOTE),      5.50));
  base.addNote(StaffNote(Note("G3", HALF_NOTE),      6.00));
  base.addNote(StaffNote(Note("B3", HALF_NOTE),      6.50));
  base.addNote(StaffNote(Note("C3", WHOLE_NOTE),     7.00));
}

int main(int argc, char **argv) {
  int bits_per_sample = 16;
  int samples_per_second = 44100;    // samples per second
  if(argc > 1) {
    std::stringstream arg(argv[1]);
    arg >> bits_per_sample;
  }

  MusicalScore score;
  read_score(score);
  unsigned int i;
  std::vector<std::vector<double>> values;
  for(i = 0; i < score.getNumberOfStaves(); i++) {
    values.push_back(std::vector<double>());
    score.renderStaff(i, samples_per_second, values[i]);
  }

  WaveFile wave("mary_score.wav", samples_per_second, bits_per_sample);
  wave.writeHeader();
  wave.writeDataSubchunkHeader();
  wave.writeValues(values);
  wave.writeSizes();
  wave.close();
  
  return 0;
}
