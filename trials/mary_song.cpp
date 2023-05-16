/*
 * Original example from here
 * https://cplusplus.com/forum/beginner/166954/
 */

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>

std::map<std::string, double> g_frequencies =
  {
   { "C4", 261.626},
   { "D4", 293.665},
   { "E4", 329.628},
   { "F4", 349.228},
   { "G4", 391.995},
   { "A4", 440.000},
   { "B4", 493.883},
  };


namespace little_endian_io
{
  template <typename Word>
  std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) ) {
    for (; size; --size, value >>= 8) {
      outs.put( static_cast <char> (value & 0xFF) );
    }
    return outs;
  }
}

void write_file_header(std::ofstream& f, int samples_per_second) {
  // Write the file headers
  f << "RIFF----WAVE"; // (chunk size to be filled in later)                     (ChunkID/ChunkSize/Format)
  f << "fmt ";         //                                                        (Subchunk1 ID)
  little_endian_io::write_word( f,     16, 4 );  // no extension data            (subchunk1 size)
  little_endian_io::write_word( f,      1, 2 );  // PCM - integer samples        (audio format)
  little_endian_io::write_word( f,      2, 2 );  // two channels (stereo file)   (num channels)
  little_endian_io::write_word( f, samples_per_second, 4 );  // samples per second (Hz)      (sample rate)
  little_endian_io::write_word( f, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8   (byte rate)
  little_endian_io::write_word( f,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)  (block align)
  little_endian_io::write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)                                (BitsPerSample)
  
}

void write_data_subchunk_header(std::ofstream& f) {
  // Write the data chunk header
  f << "data----";  // (chunk size to be filled in later)                        (subchunk2 ID/subchunk2 size)
}

void attack_and_decline(const double max_amplitude, const double hz,
                        const double attack_seconds, const double attack_min,
                        const double decline_seconds, const double decline_min,
                        std::vector<double>& amplitude) {
  const double total_seconds = attack_seconds + decline_seconds;
  const double d_size = total_seconds * hz;
  unsigned int u_size = (unsigned int)d_size;
  unsigned int u_attack_end = (unsigned int)(attack_seconds * hz);
  amplitude.resize(u_size);
  unsigned int i;
  for(i = 0; i < u_attack_end; i++) {
    amplitude[i] = attack_min + i * (max_amplitude - attack_min) / u_attack_end;
  }
  for(i = u_attack_end; i < u_size; i++) {
    amplitude[i] = max_amplitude - (i-u_attack_end) * (max_amplitude - decline_min) / (u_size-u_attack_end);
  }
}

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

Note::Note(const std::string& name, const double& duration) 
  : mName(name), mDuration(duration) {
}
const std::string& Note::getName() const {
  return mName;
}
const double& Note::getDuration() const {
  return mDuration;
}
double Note::getFrequency() const {
  double frequency = 440.0;
  auto iter = g_frequencies.find(mName);
  if(iter != g_frequencies.end()) {
    frequency = iter->second;
  }
  return frequency;
}

double notes_duration(const std::vector<Note>& notes) {
  double duration = 0.0;
  for(auto& note: notes) {
    duration += note.getDuration();
  }
  return duration;
}

void write_notes(std::ofstream& f, const std::vector<Note>& notes, const double& hz) {

  constexpr double two_pi = 6.283185307179586476925286766559;
  constexpr double max_amplitude = 32760;  // "volume"

  //double whole_note_seconds = 4.0*60.0/80.0; // q=80
  double whole_note_seconds = 4.0*60.0/120.0; // q=120
  for(auto& note: notes) {
    double frequency = note.getFrequency();
    double seconds = note.getDuration() * whole_note_seconds;

    double attack_seconds = 0.1 * seconds;
    double decline_seconds = seconds - attack_seconds;
    double attack_min = 0.0;
    double decline_min = 0.0;
    std::vector<double> amplitude;
    attack_and_decline(max_amplitude, hz, attack_seconds, attack_min, decline_seconds, decline_min, amplitude);

    int N = hz * seconds;  // total number of samples
    for (int n = 0; n < N; n++) {
      double value1 = sin((two_pi * n * frequency) / hz);
      double value2 = value1;
      // left channel
      little_endian_io::write_word(f, (int)(amplitude[n] * value1), 2);
      // right channel
      little_endian_io::write_word(f, (int)(amplitude[n] * value2), 2);
    }
  }
}


int main() {
  std::vector<Note> notes;
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

  double hz          = 44100;    // samples per second

  std::ofstream f("mary_song.wav", std::ios::binary);
  write_file_header(f, hz);
  size_t data_chunk_pos = f.tellp();
  write_data_subchunk_header(f);
  write_notes(f, notes, hz);
  
  // (We'll need the final file size to fix the chunk sizes above)
  size_t file_length = f.tellp();
  size_t data_size = file_length - (data_chunk_pos + 8);
  // Fix the data chunk header to contain the data size
  f.seekp( data_chunk_pos + 4 );
  little_endian_io::write_word( f, data_size );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  f.seekp( 0 + 4 );
  little_endian_io::write_word( f, file_length - 8, 4 );

  f.close();

  std::cout << "file_length: " << file_length << std::endl;
  std::cout << "file_length - 8: " << file_length - 8 << std::endl;
  std::cout << "data size: " << data_size << std::endl;

  return 0;
}
