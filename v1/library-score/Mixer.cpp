#include "Mixer.h"
#include <iostream>

Channel::Channel() 
  : mChannelNumber(-1) {
}

Channel::~Channel() {
}

int Channel::getChannelNumber() const {
  return mChannelNumber;
}

void Channel::setChannelNumber(const int channel_number) {
  mChannelNumber = channel_number;
}

void Channel::addStaff(const std::string& name, const double weight) {
  mStaves[name] = weight;
}

const std::map<std::string, double>& Channel::getStaves() const {
  return mStaves;
}

Mixer::Mixer() {
}

Mixer::~Mixer() {
}

void Mixer::addChannel(const Channel& channel) {
  unsigned int n = channel.getChannelNumber();
  if(n >= mChannels.size()) {
    mChannels.resize(n+1);
  }
  mChannels[n] = channel;
}

void Mixer::mix(const std::map<std::string, AudioTrack>& tracks, std::vector<AudioTrack>& channels) {
  int samples_per_second = 0;
  double seconds = 0.0;
  for(auto& track: tracks) {
    int sps = track.second.getSamplesPerSecond();
    double s = track.second.getSeconds();
    if(sps > samples_per_second) {
      samples_per_second = sps;
    }
    if(s > seconds) {
      seconds = s;
    }
  }

  channels.resize(mChannels.size());
  unsigned int i;
  for(i = 0; i < mChannels.size(); i++) {
    channels[i].setSize(samples_per_second, seconds);
    for(auto& staff: mChannels[i].getStaves()) {
      const auto& track = tracks.find(staff.first);
      if(track != tracks.end()) {
        //std::cout << "MIXER: channels["<<i<<"] " << " " << channels[i].getSeconds() << std::endl;
        //std::cout << "MIXER: track " << staff.first << " " << track->second.getSeconds() << std::endl;
        channels[i] += track->second * staff.second;
      }
    }
  }
}
