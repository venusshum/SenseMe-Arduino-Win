/* SenseMeSpeaker driver
   \author: Venus Shum
*/

#include <SenseMe.h>
#include <SenseMeSpeaker.h>
#define spkPin SPEAKER_PIN

#include <Arduino.h>
/*---------------------------------------------------------------------------*/
/**
* \brief Constructor
* 
* C++ constructor for this class. Empty.
*/
SenseMeSpeakerClass::SenseMeSpeakerClass()
{
}

void SenseMeSpeakerClass::begin() {
	pinMode(spkPin, OUTPUT);
	noTone(spkPin);
}

void SenseMeSpeakerClass::playTone(int pitch, int duration) 
{
		tone(spkPin, pitch, duration);
}


void SenseMeSpeakerClass::mute()
{
		noTone(spkPin);
}

SenseMeSpeakerClass SenseMeSpeaker=SenseMeSpeakerClass();