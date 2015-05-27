#include <SenseMePet.h>
#include <SenseMeLEDMatrix.h>
#include <SenseMeAccelerometer.h>
#include <Wire.h>


void setup() {
  // put your setup code here, to run once:
  SenseMePet.begin(80,10,10);
  SenseMeLEDMatrix.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  SenseMePet.update();
  String mood = SenseMePet.getStatus();
  
  if(mood == "happy")
  {
    SenseMeLEDMatrix.setFace("happy");
  }
  else if(mood == "sad")
  {
    SenseMeLEDMatrix.setFace("sad");
  }
  else if(mood == "tired")
  {
    SenseMeLEDMatrix.setFace("yawn");
  }
  else if(mood == "asleep")
  {
    SenseMeLEDMatrix.setFace("sleep");
  }
  delay(100);

}
