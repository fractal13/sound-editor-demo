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
#include <exception>
#include <sstream>

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

class Envelope {
public:
  Envelope(const double max_amplitude, const double attack_seconds, const double decay_seconds, const double sustain_amplitude, const double release_amplitude);
  void generateAmplitudes(std::vector<double>& amplitudes, const double seconds, const int samples_per_second) const;
private:
  double mMaximumAmplitude;
  // Attack, Decay, Sustain, Release
  double mAttackSeconds;
  double mDecaySeconds;
  double mSustainAmplitude;
  double mReleaseSeconds;
};

Envelope::Envelope(const double max_amplitude, const double attack_seconds, const double decay_seconds, const double sustain_amplitude, const double release_seconds)
  : mMaximumAmplitude(max_amplitude), mAttackSeconds(attack_seconds), mDecaySeconds(decay_seconds), mSustainAmplitude(sustain_amplitude), mReleaseSeconds(release_seconds) {
}

void Envelope::generateAmplitudes(std::vector<double>& amplitudes, const double seconds, const int samples_per_second) const {
  if(seconds < mAttackSeconds + mDecaySeconds + mReleaseSeconds) {
    throw std::exception();
  }
  int N = samples_per_second * seconds;  // total number of samples
  amplitudes.resize(N);
  int attack_n = samples_per_second * mAttackSeconds;
  int decay_n = attack_n + samples_per_second * mDecaySeconds;
  int sustain_n = N - samples_per_second * mReleaseSeconds;
  int release_n = N;
  int i;
  double m, x, b;

  // attack from 0 to full
  b = 0.0;
  m = mMaximumAmplitude / (attack_n - 0);
  for(i = 0; i < attack_n; i++) {
    x = i;
    amplitudes[i] = m*x+b;
  }

  // decay from full to sustain
  b = mMaximumAmplitude;
  m = (mSustainAmplitude - mMaximumAmplitude) / (decay_n - attack_n);
  for(i = attack_n; i < decay_n; i++) {
    x = i-attack_n;
    amplitudes[i] = m*x+b;
  }
  
  // sustain
  b = mSustainAmplitude;
  m = 0;
  for(i = decay_n; i < sustain_n; i++) {
    x = i-decay_n;
    amplitudes[i] = m*x+b;
  }
  
  // decay from sustain to 0.0 (release)
  b = mSustainAmplitude;
  m = (0 - mSustainAmplitude) / (release_n - sustain_n);
  for(i = sustain_n; i < release_n; i++) {
    x = i-sustain_n;
    amplitudes[i] = m*x+b;
  }
}


/*
 * Envelope end
 *******************************************************************/


/*******************************************************************
 * Instrument file start
 */
constexpr double two_pi = 6.283185307179586476925286766559;

class Instrument {
public:
  Instrument();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
private:
};

Instrument::Instrument() {
}

void Instrument::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {
  
  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double value1 = std::sin((two_pi * n * frequency) / samples_per_second);
    samples[n] = value1;
  }
}

class Instrument2: public Instrument {
public:
  Instrument2();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
private:
};

Instrument2::Instrument2() 
  : Instrument() {
}

void Instrument2::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {

  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double value1 = std::cos((two_pi * n * frequency) / samples_per_second);
    if(value1 > 0.5) {
      value1 = 1.0;
    } else if(value1 < -0.5) {
      value1 = -1.0;
    }
    samples[n] = value1;
  }
}

class Instrument3: public Instrument {
public:
  Instrument3();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
private:
};

Instrument3::Instrument3() 
  : Instrument() {
}

void Instrument3::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {

  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double angle = (two_pi * n * frequency) / samples_per_second;
    double position = std::fmod(angle, two_pi)/two_pi;
    double value = 0.0;
    if(position < 0.25) {
      double m = (1.0/0.25);
      double b = 0.0;
      double x = (position - 0.0);
      value = m*x+b;
    } else if(position < 0.75) {
      double m = (-2.0/0.5);
      double b = 1.0;
      double x = (position - 0.25);
      value = m*x+b;
    } else {
      double m = (1.0/0.25);
      double b = -1.0;
      double x = (position - 0.75);
      value = m*x+b;
    }
    samples[n] = value;
  }
}

class Instrument4: public Instrument {
public:
  Instrument4();
  virtual void generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const;
private:
};

Instrument4::Instrument4() 
  : Instrument() {
}

void Instrument4::generateSamples(std::vector<double>& samples, const double frequency, const double seconds, const int samples_per_second) const {

  int N = samples_per_second * seconds;  // total number of samples
  samples.resize(N);
  for (int n = 0; n < N; n++) {
    double angle = (two_pi * n * frequency) / samples_per_second;
    double position = std::fmod(angle, two_pi)/two_pi;
    double value = 0.0;
    if(position < 0.5) {
      value = 1.0;
    } else {
      value = -1.0;
    }
    samples[n] = value;
  }
}

/*
 * Instrument end
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
  WaveFile(const std::string& filename, int samples_per_second, int bits_per_sample);
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
  int mBitsPerSample;
  int mMaximumAmplitude;
  int mBytesPerSample;
};

WaveFile::WaveFile(const std::string& filename, int samples_per_second, int bits_per_sample)
  : mFile(filename, std::ios::binary), mSamplesPerSecond(samples_per_second), mDataSubchunkPosition(0), mBitsPerSample(bits_per_sample) {
  mMaximumAmplitude = (1<<(mBitsPerSample-1)) - 1;
  mBytesPerSample = mBitsPerSample/8;
  switch(mBitsPerSample) {
  case 8:
    /* fall through */
  case 16:
    /* fall through */
  case 24:
    /* fall through */
  case 32:
    /* all allowed */
    break;
  default:
    throw std::exception();
    break;
  }
}

WaveFile::~WaveFile() {
  if(mFile) {
    mFile.close();
  }
}

void WaveFile::writeHeader() {
  int byte_rate = (mSamplesPerSecond * mBitsPerSample * 2) / 8;
  int data_block_size = (mBitsPerSample * 2) / 8;
  // Write the file header
  mFile << "RIFF----WAVE"; // (chunk size to be filled in later)                     (ChunkID/ChunkSize/Format)
  mFile << "fmt ";         //                                                        (Subchunk1 ID)
  little_endian_io::write_word( mFile,                16, 4 );  // no extension data            (subchunk1 size)
  little_endian_io::write_word( mFile,                 1, 2 );  // PCM - integer samples        (audio format)
  little_endian_io::write_word( mFile,                 2, 2 );  // two channels (stereo file)   (num channels)
  little_endian_io::write_word( mFile, mSamplesPerSecond, 4 );  // samples per second (Hz)      (sample rate)
  little_endian_io::write_word( mFile,         byte_rate, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8   (byte rate)
  little_endian_io::write_word( mFile,   data_block_size, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)  (block align)
  little_endian_io::write_word( mFile,    mBitsPerSample, 2 );  // number of bits per sample (use a multiple of 8)                                (BitsPerSample)
}

void WaveFile::writeDataSubchunkHeader() {
  mDataSubchunkPosition = mFile.tellp();
  // Write the data chunk header
  mFile << "data----";  // (chunk size to be filled in later)                        (subchunk2 ID/subchunk2 size)
}

void WaveFile::writeNotes(const std::vector<Note>& notes) {
  double max_amplitude = mMaximumAmplitude;  // "volume"
  double attack_seconds  = 0.10;
  double decay_seconds   = 0.05;
  double release_seconds = 0.10;
  double sustain_amplitude = max_amplitude * 0.5;
  Instrument4 instrument;
  Envelope envelope(max_amplitude, attack_seconds, decay_seconds, sustain_amplitude, release_seconds);
  //double whole_note_seconds = 4.0*60.0/80.0; // q=80
  double whole_note_seconds = 4.0*60.0/120.0; // q=120
  for(auto& note: notes) {
    double frequency = note.getFrequency();
    double seconds = note.getDuration() * whole_note_seconds;

    std::vector<double> amplitudes;
    envelope.generateAmplitudes(amplitudes, seconds, mSamplesPerSecond);
    std::vector<double> samples;
    instrument.generateSamples(samples, frequency, seconds, mSamplesPerSecond);

    int N = mSamplesPerSecond * seconds;  // total number of samples
    for (int n = 0; n < N; n++) {
      double value1 = samples[n];
      double value2 = value1;
      // left channel
      little_endian_io::write_word(mFile, (int)(amplitudes[n] * value1), mBytesPerSample);
      // right channel
      little_endian_io::write_word(mFile, (int)(amplitudes[n] * value2), mBytesPerSample);
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
