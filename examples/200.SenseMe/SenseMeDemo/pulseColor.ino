// pulse the Neopixel LED in turns with RGB
uint8_t delayval=5;
void pulseColor(uint8_t r, uint8_t g, uint8_t b) {

  for(int i=0;i<255;i++){
    // Neopixel.Color takes RGB values, from 0,0,0 up to 255,255,255
    SenseMeNeopixel.setBrightness(i),
    SenseMeNeopixel.setColor(r,g,b); // Moderately bright green color.
    SenseMeNeopixel.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
  
  for(int i=255;i>=0;i--){
    // Neopixel.Color takes RGB values, from 0,0,0 up to 255,255,255
    SenseMeNeopixel.setBrightness(i),
    SenseMeNeopixel.setColor(r,g,b); // Moderately bright green color.
    SenseMeNeopixel.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
  
}
