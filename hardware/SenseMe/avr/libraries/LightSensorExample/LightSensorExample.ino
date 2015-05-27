#include <SenseMeLightSensor.h>
#include <Wire.h>
#include <SenseMeLEDMatrix.h>
#include <SenseMeNeopixel.h>


int y[8];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("setup");
  SenseMeLightSensor.begin();
  SenseMeLEDMatrix.begin();
  SenseMeNeopixel.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = SenseMeLightSensor.lightLevel();;
  Serial.println(a);
  SenseMeLEDMatrix.clear();
  int b = a/128;
  if (b > 7)
  {
    b = 7;
  }
  for(int i=0; i<7; i++)
  {
    y[i]= y[i+1];
    SenseMeLEDMatrix.drawPixel(i, y[i], LED_ON);
  }
  y[7] = 7-b;
  SenseMeLEDMatrix.drawPixel(7, y[7], LED_ON);
  SenseMeLEDMatrix.writeDisplay();
  
  int c = a/4;
  if(c>255)
  {
    c = 255;
  }
  SenseMeNeopixel.setColor(127,0,127, c);
  
  delay(1000);

}
