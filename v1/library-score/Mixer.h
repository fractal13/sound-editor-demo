#ifndef _MIXER_H_
#define _MIXER_H_

#include "AudioTrack.h"
#include <map>
#include <vector>
#include <string>

class Channel {
public:
  Channel();
  virtual ~Channel();
  
  int getChannelNumber() const;
  void setChannelNumber(const int channel_number);
  void addStaff(const std::string& name, const double weight);
  
  const std::map<std::string, double>& getStaves() const;

protected:
  int mChannelNumber;
  std::map<std::string, double> mStaves;

private:
};

class Mixer {
public:
  Mixer();
  virtual ~Mixer();

  void addChannel(const Channel& channel);
  
  void mix(const std::map<std::string, AudioTrack>& tracks, std::vector<AudioTrack>& channels);

  const std::vector<Channel>& getChannels() const;

protected:
  std::vector<Channel> mChannels;
private:
};

#endif /* _MIXER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
