#ifndef _TIMESIGNATURE_H_
#define _TIMESIGNATURE_H_

class TimeSignature {
public:
  TimeSignature(); // defaults to 4/4 time
  TimeSignature(const int beats_per_bar, const int beat_value);
  int getBeatsPerBar() const;
  int getBeatValue() const;
  void setBeatsPerBar(const int beats_per_bar);
  void setBeatValue(const int beat_value);
private:
  int mBeatsPerBar; // Number of beats per bar
  int mBeatValue;   // Note that indicates a beat (1 == whole, 2 == half, etc.)
};


#endif /* _TIMESIGNATURE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
