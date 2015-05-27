/*
  Melody
 
 Plays a melody on SenseMe cube
 
 */
 #include <SenseMe.h>
 #include <SenseMeSpeaker.h>  



// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };


void setup() {
  SenseMeSpeaker.begin();
  littleStar();
  delay(5000);  
}

void loop() {
  // no need to repeat the melody.

  
}

void playMelody1(){
    // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    SenseMeSpeaker.playTone(melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    SenseMeSpeaker.mute();
  }
  

}

#define speakerOut SPEAKER_PIN

const int  R=0;
int melody1[] = { NOTE_C5,  NOTE_C5,  NOTE_G5,  NOTE_G5,  NOTE_A5, NOTE_A5,  NOTE_G5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5} ;
int beats1[]  = { 4,4,4,4,4,4,2, 4, 4, 4, 4, 4, 4, 1 } ; 


void littleStar(){
    // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 14; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/beats1[thisNote];
    SenseMeSpeaker.playTone(melody1[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    SenseMeSpeaker.mute();
  }
  

}


