#include <Wire.h>
#include <SenseMeThermistor.h>
#include <SenseMeLEDMatrix.h>
#include <SenseMe.h>
/*
  Thermistor LED Matrix Display
  Read the values NTC100 thermistor and display the value on the LED matrix on SenseMeCube

  modified 16/4/2015
  by Venus Shum
 */
 
  
void setup()
{
  Serial.begin(115200);
  SenseMeThermistor.begin();
  SenseMeLEDMatrix.begin();
  SenseMeLEDMatrix.clear();
}



void loop()
{   
    tempLight();
}

void tempLight() {
    // Get the temperature
    //
    float t = SenseMeThermistor.temperature(CELSIUS);
    
    // Print the temperature
    Serial.print("Temp is: ");
    Serial.println(t);
    
    delay(1000);
    
    // Display the temperature on the LED Matrix
    
    SenseMeLEDMatrix.setTextSize(1);
    SenseMeLEDMatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
    SenseMeLEDMatrix.setTextColor(LED_ON);
    for (int8_t x=0; x>=-40; x--) {
    SenseMeLEDMatrix.clear();
    SenseMeLEDMatrix.setCursor(x,0);
    SenseMeLEDMatrix.print(t);
    SenseMeLEDMatrix.print("C");
    SenseMeLEDMatrix.writeDisplay();
    delay(100);
  }
}



