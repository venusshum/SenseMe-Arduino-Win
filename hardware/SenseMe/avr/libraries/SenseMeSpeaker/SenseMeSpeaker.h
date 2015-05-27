#include <pitches.h>


class SenseMeSpeakerClass 
{
  public:
	 SenseMeSpeakerClass();
	 void begin(void);
     void playTone(int, int);
	 void mute();
  private:
 
};

extern SenseMeSpeakerClass SenseMeSpeaker;