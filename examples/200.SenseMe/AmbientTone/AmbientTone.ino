/*
  Pitch follower
 
 Plays a pitch that changes based on a changing analog input
 
 circuit:
 * 8-ohm speaker on digital pin 8
 * photoresistor on analog 0 to 5V
 * 4.7K resistor on analog 0 to ground
 
 created 21 Jan 2010
 modified 31 May 2012
 by Tom Igoe, with suggestion from Michael Flynn

This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/Tone2
 
 */

#include <Wire.h>
#include <SHT2x.h>
#include <SenseMeLightSensor.h>
#include <SenseMe.h>
#include "pitches.h"

int melody1[] = { NOTE_C5,  NOTE_C5,  NOTE_G5,  NOTE_G5,  NOTE_A5, NOTE_A5,  NOTE_G5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5} ;
int beats1[]  = { 4,4,4,4,4,4,2, 4, 4, 4, 4, 4, 4, 1 } ; 

void setup() {
  // initialize serial communications (for debugging only):
  Wire.begin();
  Serial.begin(9600);
  SenseMeLightSensor.begin();
  
  littleStar();
  delay(5000);
}

void loop() {
  // read the sensor:
  int sensorReading = SHT2x.GetHumidity();
  // print the sensor reading so you know its range
    uint16_t l;
  Serial.print("Humidity(%RH): ");
  Serial.print(sensorReading);
  Serial.print("     Temperature(C): ");
  Serial.print(SHT2x.GetTemperature());
  
  Serial.print("     Light Value: ");
  l=SenseMeLightSensor.lightLevel();
  Serial.println(l);
  // map the analog input range (in this case, 400 - 1000 from the photoresistor)
  // to the output pitch range (120 - 1500Hz)
  // change the minimum and maximum input numbers below
  // depending on the range your sensor's giving:
  int thisPitch = map(sensorReading, 0, 100, 120, 2500);

  // play the pitch:
  tone(SPEAKER_PIN, thisPitch, 10);
  delay(1);        // delay in between reads for stability
}

void littleStar(){
    // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 14; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/beats1[thisNote];
    tone(SPEAKER_PIN, melody1[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}





