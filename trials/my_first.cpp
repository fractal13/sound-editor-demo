/*
 * Original example from here
 * https://cplusplus.com/forum/beginner/166954/
 */

#include <cmath>
#include <fstream>
#include <iostream>

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

int main() {
  std::ofstream f( "my_first.wav", std::ios::binary );

  // Write the file headers
  f << "RIFF----WAVE"; // (chunk size to be filled in later)                     (ChunkID/ChunkSize/Format)
  f << "fmt ";         //                                                        (Subchunk1 ID)
  little_endian_io::write_word( f,     16, 4 );  // no extension data            (subchunk1 size)
  little_endian_io::write_word( f,      1, 2 );  // PCM - integer samples        (audio format)
  little_endian_io::write_word( f,      2, 2 );  // two channels (stereo file)   (num channels)
  little_endian_io::write_word( f,  44100, 4 );  // samples per second (Hz)      (sample rate)
  little_endian_io::write_word( f, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8   (byte rate)
  little_endian_io::write_word( f,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)  (block align)
  little_endian_io::write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)                                (BitsPerSample)

  // Write the data chunk header
  size_t data_chunk_pos = f.tellp();
  f << "data----";  // (chunk size to be filled in later)                        (subchunk2 ID/subchunk2 size)
  
  // Write the audio samples
  // (We'll generate a single C4 note with a sine wave, fading from left to right)
  constexpr double two_pi = 6.283185307179586476925286766559;
  constexpr double max_amplitude = 32760;  // "volume"

  double hz        = 44100;    // samples per second
  double frequency1 = 261.626;  // middle C
  double frequency2 = 523.251;  // C5?
  double seconds   = 2.5;      // time

  int N = hz * seconds;  // total number of samples
  for (int n = 0; n < N; n++) {
    double amplitude = (double)n / N * max_amplitude;
    double value1     = sin( (two_pi * n * frequency1) / hz );
    double value2     = sin( (two_pi * n * frequency2) / hz );
    // left channel
    little_endian_io::write_word( f, (int)(                 amplitude  * value1), 2 );
    // right channel
    little_endian_io::write_word( f, (int)((max_amplitude - amplitude) * value2), 2 );
  }
  
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
