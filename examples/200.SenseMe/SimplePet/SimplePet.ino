#include <Wire.h>
#include <SenseMeLEDMatrix.h>
#include <SenseMeAccelerometer.h>
#include <SenseMeNeopixel.h>
#include <SenseMeLightSensor.h>

float accel[3];

void setup() {
  // put your setup code here, to run once:
  SenseMeLEDMatrix.begin(); //initialise matrix
  SenseMeAccelerometer.begin(); //initialise accelerometer
}

void loop() {
  // put your main code here, to run repeatedly:
  long randNum = random(100); // generates a random number between 1 and 100 to decide how your pet is feeling
  if (randNum < 80)
  {
    SenseMeLEDMatrix.setFace("happy"); //if the number is below 80 your pet will be happy :)
    delay(1000); //wait for 1 second
  }
  else if(randNum < 90)
  {
    SenseMeLEDMatrix.setFace("sad"); //if the number is between 80 and 90 your pet will become sad :(
    boolean sad = true; // this makes a variable called sad which can either be true or false (using the word boolean means that it can be true or false)
    while(sad) //this loops for as long as your pet is sad
    {
      delay(50); // this waits for one twentieth of a second in preparation for taking a measurement from the accelerometer
      float absAccel = SenseMeAccelerometer.magnitude(); // this return the amount that the cube is accelerating. 1 is the acceleration due to gravity more than 1 means that you are shaking it
      if(absAccel>1.5)// this tests whether the cube is moving enough
      {
        SenseMeLEDMatrix.setFace("laugh"); // if the cube is moving then your pet will laugh
        delay(1000); // wait for 1 second
        sad = false; // your pet is no longer sad 
      }
      // if there is no movement then you pet continues to be sad  
    }
  }
  else 
  {
    SenseMeLEDMatrix.setFace("yawn"); // if the number is between 90 and 100 then you pet is tired :O
    boolean upright = true;
    while(upright) // this loops while the cube is sitting upright making the code have to wait until the cube is laid down
    {
      SenseMeAccelerometer.xyz(accel); // this gathers the co-ordinates from the accelerometer
      if(accel[0]>-0.1) // this tests whether the cube has been laid down
      {
        upright = false;
      } 
    }
    SenseMeLEDMatrix.setFace("sleep");// if your pet is laid on his back then he will go to sleep
    delay(1000);
    boolean asleep = true;
    while(asleep)// this will loop for as long as your pet is asleep (laid on his back)
    {
      SenseMeAccelerometer.xyz(accel);
      if(accel[0]<-0.9)
      {
        asleep = false; // if you pet is put upright then he wakes up
      }  
    }
  }
  //now we go back to the start of the loop
}


