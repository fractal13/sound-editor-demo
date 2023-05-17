#include "Note.h"
#include "WaveFile.h"
#include <vector>
#include <sstream>

void read_notes(std::vector<Note>& notes) {
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("C4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("E4", HALF_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("D4", HALF_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("G4", QUARTER_NOTE));
  notes.push_back(Note("G4", HALF_NOTE));

  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("C4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("E4", QUARTER_NOTE));
  notes.push_back(Note("D4", QUARTER_NOTE));
  notes.push_back(Note("C4", WHOLE_NOTE));
}

int main(int argc, char **argv) {
  int bits_per_sample = 16;
  if(argc > 1) {
    std::stringstream arg(argv[1]);
    arg >> bits_per_sample;
  }

  std::vector<Note> notes;
  read_notes(notes);

  int samples_per_second = 44100;    // samples per second
  WaveFile wave("mary_refactor.wav", samples_per_second, bits_per_sample);
  wave.writeHeader();
  wave.writeDataSubchunkHeader();
  wave.writeNotes(notes);
  wave.writeSizes();
  wave.close();

  return 0;
}
