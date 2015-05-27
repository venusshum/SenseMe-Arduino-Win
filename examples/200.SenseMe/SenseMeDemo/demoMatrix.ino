//function display hello world. 
void demoMatrix() {
  SenseMeLEDMatrix.setTextSize(1);
  SenseMeLEDMatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  SenseMeLEDMatrix.setTextColor(LED_ON);
  for (int8_t x=0; x>=-36; x--) {
    SenseMeLEDMatrix.clear();
    SenseMeLEDMatrix.setCursor(x,0);
    SenseMeLEDMatrix.print("Hello");
    SenseMeLEDMatrix.writeDisplay();
    delay(100);
  }
  SenseMeLEDMatrix.setRotation(3);
  
  for (int8_t x=7; x>=-36; x--) {
    SenseMeLEDMatrix.clear();
    SenseMeLEDMatrix.setCursor(x,0);
    SenseMeLEDMatrix.print("World");
    SenseMeLEDMatrix.writeDisplay();
    delay(100);
  }
  SenseMeLEDMatrix.setRotation(0);
}
