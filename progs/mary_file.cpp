#include "MusicalScore.h"
#include "WaveFile.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>

void parse_note(const std::string& word, Note& note) {
  // qE4, w.Bb2, etc
  double extension = 1.0;
  unsigned int first_note = 1;

  if(word[1] == '.') {
    extension = 1.5;
    first_note = 2;
  }

  switch(word[0]) {
  case 'e':
    note.setDuration(EIGHTH_NOTE*extension);
    break;
  case 'q':
    note.setDuration(QUARTER_NOTE*extension);
    break;
  case 'h':
    note.setDuration(HALF_NOTE*extension);
    break;
  case 'w':
    note.setDuration(WHOLE_NOTE*extension);
    break;
  }
  
  note.setName(word.substr(first_note));
}

void read_staff(std::istream& is, MusicalStaff& staff) {
  // assumes that STAFF keyword has been consumed
  // reads staff header, then notes until STAFF-END is encountered
  std::string word;

  Instrument *instrument = 0;
  is >> word; // staff name, FIXME
  is >> word;
  if(word == "instrument") {
    instrument = new Instrument;
  } else if(word == "instrument2") {
    instrument = new Instrument2;
  } else if(word == "instrument3") {
    instrument = new Instrument3;
  } else if(word == "instrument4") {
    instrument = new Instrument4;
  }
  staff.setInstrument(instrument);
  
  while(is >> word) {
    if(word == "STAFF-END") {
      break;
    }
    std::stringstream wstream;
    double start;
    wstream.str(word);
    wstream >> start;
    is >> word;
    Note note;
    parse_note(word, note);
    StaffNote staff_note(note, start);
    staff.addNote(staff_note);
  }
}

void read_score(std::istream& is, MusicalScore& score) {
  std::string word;
  int         integer;
  while(is) {
    is >> word;
    if(!is) { break; }
    if(word == "SCORE") {
      is >> integer;
      score.getTimeSignature().setBeatsPerBar(integer);
      is >> integer;
      score.getTimeSignature().setBeatValue(integer);
      is >> integer;
      score.setTempo(integer);
    } else if(word == "STAFF") {
      unsigned int staff_index = score.createStaff();
      MusicalStaff& staff = score.getStaff(staff_index);
      read_staff(is, staff);
    } else {
      std::stringstream ss;
      ss << "Expected a keyword, but got: '" << word << "'.";
      throw std::invalid_argument(ss.str());
    }
  }
}

int main(int argc, char **argv) {
  int bits_per_sample = 16;
  int samples_per_second = 44100;    // samples per second
  if(argc > 1) {
    std::stringstream arg(argv[1]);
    arg >> bits_per_sample;
  }
  std::string filename = "../doc/mary-had-a-little-lamb.score";
  if(argc > 2) {
    std::stringstream arg(argv[2]);
    arg >> filename;
  }

  MusicalScore score;
  std::ifstream f(filename);
  read_score(f, score);
  f.close();
  
  unsigned int i;
  std::vector<std::vector<double>> values;
  for(i = 0; i < score.getNumberOfStaves(); i++) {
    values.push_back(std::vector<double>());
    score.renderStaff(i, samples_per_second, values[i]);
  }

  WaveFile wave("mary_file.wav", samples_per_second, bits_per_sample);
  wave.writeHeader();
  wave.writeDataSubchunkHeader();
  wave.writeValues(values);
  wave.writeSizes();
  wave.close();
  
  return 0;
}
