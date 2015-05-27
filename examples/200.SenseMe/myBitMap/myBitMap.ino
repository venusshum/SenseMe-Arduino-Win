#include <SenseMeLEDMatrix.h>
#include <Wire.h>

static const uint8_t PROGMEM
  myBitmap[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 };
    
void setup() {
  // put your setup code here, to run once:
  SenseMeLEDMatrix.begin();  
  SenseMeLEDMatrix.clear();
  SenseMeLEDMatrix.writeDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  SenseMeLEDMatrix.clear();
  SenseMeLEDMatrix.writeDisplay();
  delay(1000);
  SenseMeLEDMatrix.drawBitmap(0, 0, myBitmap, 8, 8, LED_ON);
  SenseMeLEDMatrix.writeDisplay();
  delay(1000);
  
}
