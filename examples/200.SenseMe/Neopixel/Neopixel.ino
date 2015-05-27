#include <SenseMeNeopixel.h>

void setup() {
  // put your setup code here, to run once:
  SenseMeNeopixel.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  SenseMeNeopixel.setColor(255,255,255, 15);
}
