// function display a face depanding on the value of ran
void luckyFace(int ran){
  switch (ran) {
    case 0:
      SenseMeLEDMatrix.clear();
      SenseMeLEDMatrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
      pulseColor(255,255,0);
      //delay(1000);
      for (int8_t x=0; x>=-(6*6); x--) {
        SenseMeLEDMatrix.clear();
        SenseMeLEDMatrix.setCursor(x,0);
        SenseMeLEDMatrix.print("Lucky");
        SenseMeLEDMatrix.writeDisplay();
        delay(100);
      }
      //delay(500);
      break;
   case 1:
      SenseMeLEDMatrix.clear();
      SenseMeLEDMatrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
      pulseColor(255,255,255);
      //delay(1000);
      for (int8_t x=0; x>=-(4*6); x--) {
        SenseMeLEDMatrix.clear();
        SenseMeLEDMatrix.setCursor(x,0);
        SenseMeLEDMatrix.print("OK!");
        SenseMeLEDMatrix.writeDisplay();
        delay(100);
      }
      //delay(500);
      break;
  case 2:
      SenseMeLEDMatrix.clear();
      SenseMeLEDMatrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
      pulseColor(0,255,255);
      //delay(1000);
      for (int8_t x=0; x>=-(6*6); x--) {
        SenseMeLEDMatrix.clear();
        SenseMeLEDMatrix.setCursor(x,0);
        SenseMeLEDMatrix.print("Sad..");
        SenseMeLEDMatrix.writeDisplay();
        delay(100);
      }
      //delay(500);
      break;
  default:
      SenseMeLEDMatrix.clear();
      SenseMeLEDMatrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
      //delay(1000);
      for (int8_t x=0; x>=-(15*6); x--) {
        SenseMeLEDMatrix.clear();
        SenseMeLEDMatrix.setCursor(x,0);
        SenseMeLEDMatrix.print("Lucky");
        SenseMeLEDMatrix.writeDisplay();
        delay(100);
      }
  }
  
}
