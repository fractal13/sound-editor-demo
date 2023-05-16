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


/*******************************************************************
 * Note class start
 */
const std::map<std::string, double> g_frequencies =
  {
   { "C3", 130.813 },
   { "D3", 146.832 },
   { "E3", 164.814 },
   { "F3", 174.614 },
   { "G3", 195.998 },
   { "A3", 220.000 },
   { "B3", 246.942 },
   { "C4", 261.626 },
   { "D4", 293.665 },
   { "E4", 329.628 },
   { "F4", 349.228 },
   { "G4", 391.995 },
   { "A4", 440.000 },
   { "B4", 493.883 },
  };

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
  double frequency = 440.000;
  auto iter = g_frequencies.find(mName);
  if(iter != g_frequencies.end()) {
    frequency = iter->second;
  }
  return frequency;
}

/*
 * Note class end
 *******************************************************************/

/*******************************************************************
 * Envelope start
 */

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


/*
 * Envelope end
 *******************************************************************/

/*******************************************************************
 * Wav file start
 */


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

class WaveFile {
public:
  WaveFile(const std::string& filename, int samples_per_second);
  ~WaveFile();

  void writeHeader();
  void writeDataSubchunkHeader();
  void writeNotes(const std::vector<Note>& notes);
  void writeSizes();
  void close();

private:
  std::ofstream mFile;
  int mSamplesPerSecond;
  size_t mDataSubchunkPosition;
};

WaveFile::WaveFile(const std::string& filename, int samples_per_second)
  : mFile(filename, std::ios::binary), mSamplesPerSecond(samples_per_second), mDataSubchunkPosition(0) {
}

WaveFile::~WaveFile() {
  if(mFile) {
    mFile.close();
  }
}

void WaveFile::writeHeader() {
  // Write the file header
  mFile << "RIFF----WAVE"; // (chunk size to be filled in later)                     (ChunkID/ChunkSize/Format)
  mFile << "fmt ";         //                                                        (Subchunk1 ID)
  little_endian_io::write_word( mFile,     16, 4 );  // no extension data            (subchunk1 size)
  little_endian_io::write_word( mFile,      1, 2 );  // PCM - integer samples        (audio format)
  little_endian_io::write_word( mFile,      2, 2 );  // two channels (stereo file)   (num channels)
  little_endian_io::write_word( mFile, mSamplesPerSecond, 4 );  // samples per second (Hz)      (sample rate)
  little_endian_io::write_word( mFile, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8   (byte rate)
  little_endian_io::write_word( mFile,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)  (block align)
  little_endian_io::write_word( mFile,     16, 2 );  // number of bits per sample (use a multiple of 8)                                (BitsPerSample)
}

void WaveFile::writeDataSubchunkHeader() {
  mDataSubchunkPosition = mFile.tellp();
  // Write the data chunk header
  mFile << "data----";  // (chunk size to be filled in later)                        (subchunk2 ID/subchunk2 size)
}

void WaveFile::writeNotes(const std::vector<Note>& notes) {
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
    attack_and_decline(max_amplitude, mSamplesPerSecond, attack_seconds, attack_min, decline_seconds, decline_min, amplitude);

    int N = mSamplesPerSecond * seconds;  // total number of samples
    for (int n = 0; n < N; n++) {
      double value1 = sin((two_pi * n * frequency) / mSamplesPerSecond);
      double value2 = value1;
      // left channel
      little_endian_io::write_word(mFile, (int)(amplitude[n] * value1), 2);
      // right channel
      little_endian_io::write_word(mFile, (int)(amplitude[n] * value2), 2);
    }
  }
}

void WaveFile::writeSizes() {
  size_t file_length = mFile.tellp();
  size_t data_size = file_length - (mDataSubchunkPosition + 8);
  // Fix the data chunk header to contain the data size
  mFile.seekp(mDataSubchunkPosition + 4);
  little_endian_io::write_word(mFile, data_size);
  
  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  mFile.seekp(0 + 4);
  little_endian_io::write_word(mFile, file_length - 8, 4);
}

void WaveFile::close() {
  if(mFile) {
    mFile.close();
  }
}


/*
 * Wav file end
 *******************************************************************/


double notes_duration(const std::vector<Note>& notes) {
  double duration = 0.0;
  for(auto& note: notes) {
    duration += note.getDuration();
  }
  return duration;
}


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

int main() {
  std::vector<Note> notes;
  read_notes(notes);

  double hz          = 44100;    // samples per second
  WaveFile wave("mary_refactor.wav", hz);
  wave.writeHeader();
  wave.writeDataSubchunkHeader();
  wave.writeNotes(notes);
  wave.writeSizes();
  wave.close();

  return 0;
}
