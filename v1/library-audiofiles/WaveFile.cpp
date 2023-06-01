#include "WaveFile.h"
#include "Instrument.h"
#include "Envelope.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <vector>


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
    std::stringstream ss;
    ss << "bits per sample must be a multiple of 8.";
    throw std::invalid_argument(ss.str());
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

void WaveFile::writeTracks(const std::vector<AudioTrack>& tracks) {
  if(tracks.size() != 2) {
    // hack stereo for now, still need a mixer
    std::stringstream ss;
    ss << "WaveFile::writeTracks: ";
    ss << "Only support exactly 2 staves for now.";
    throw std::invalid_argument(ss.str());
  }
  if(tracks[0].size() != tracks[1].size()) {
    // hack same same
    std::stringstream ss;
    ss << "Both streams must have same number of values.";
    throw std::invalid_argument(ss.str());
  }
  unsigned int n;
  for(n = 0; n < tracks[0].size(); n++) {
    little_endian_io::write_word(mFile, (int)(tracks[0][n] * mMaximumAmplitude), mBytesPerSample);
    little_endian_io::write_word(mFile, (int)(tracks[1][n] * mMaximumAmplitude), mBytesPerSample);
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

