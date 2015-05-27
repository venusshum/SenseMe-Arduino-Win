//Display the Temperature on LED Matrix

void tempLight() {
    // Get the temperature
    //
    //float temp = SenseMeThermistor.temperature(CELSIUS);
    float temp = SHT2x.GetTemperature();
    // Print the temperature
    Serial.print("Temp is: ");
    Serial.println(temp);
    
    //delay(1000);
    
    // Choose the next point in RGB space by temperature - this is a 
    // linear scale that maps from pure blue for the coldest to pure
    // red for the hottest, with intermediate temperatures
    // represented by the appropriate mixture of blue and red.
    //
    // We constrain the temperature so that the lowest temperature we
    // represent is TBLUE and the highest is TRED. MAX_BRIGHTNESS is
    // the maximum we can set an LED channel to.
    //
    
    SenseMeLEDMatrix.setTextSize(1);
    SenseMeLEDMatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
    SenseMeLEDMatrix.setTextColor(LED_ON);
    for (int8_t x=0; x>=-40; x--) {
      SenseMeLEDMatrix.clear();
      SenseMeLEDMatrix.setCursor(x,0);
      SenseMeLEDMatrix.print(temp);
      SenseMeLEDMatrix.print("C");
      SenseMeLEDMatrix.writeDisplay();
    delay(100);
  }
}
