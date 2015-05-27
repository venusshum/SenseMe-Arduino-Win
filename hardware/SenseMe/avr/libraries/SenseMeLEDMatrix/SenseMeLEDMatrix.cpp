#include <SenseMeLEDMatrix.h>

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100},
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 },
   laugh_bmp[] =
  { B00111100,
    B01100110,
    B11011011,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
    yawn_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01011010,
    B00111100 },
    sleep_bmp[] =
  { B00000001,
    B00000011,
    B00011100,
    B00001000,
    B00011100,
    B11100000,
    B01000000,
    B11100000 }
      ;

SenseMeLEDMatrixClass::SenseMeLEDMatrixClass(void) : Adafruit_8x8matrix() {
}

void SenseMeLEDMatrixClass::begin(void) {
  i2c_addr = 0x70;

  Wire.begin();

  Wire.beginTransmission(i2c_addr);
  Wire.write(0x21);  // turn on oscillator
  Wire.endTransmission();
  blinkRate(HT16K33_BLINK_OFF);
  
  setBrightness(15); // max brightness
}

void SenseMeLEDMatrixClass::setFace(String mood){
  SenseMeLEDMatrix.clear();
  if(mood== "happy"){
      SenseMeLEDMatrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
  }
  else if (mood == "sad"){
      SenseMeLEDMatrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
  }
  else if (mood == "neutral"){
      SenseMeLEDMatrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
  }
  else if (mood == "laugh"){
      SenseMeLEDMatrix.drawBitmap(0, 0, laugh_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
  }
  else if (mood == "sleep"){
      SenseMeLEDMatrix.drawBitmap(0, 0, sleep_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
  }
  else if (mood == "yawn"){
      SenseMeLEDMatrix.drawBitmap(0, 0, yawn_bmp, 8, 8, LED_ON);
      SenseMeLEDMatrix.writeDisplay();
  }
}

void SenseMeLEDMatrixClass::scrollText(String msg) {
	SenseMeLEDMatrix.clear();
	SenseMeLEDMatrix.setTextWrap(false);
	SenseMeLEDMatrix.setTextColor(LED_ON);
	
	int len = msg.length();
	for (int8_t x=0; x>=-len * 6; x--) {
      SenseMeLEDMatrix.clear();
      SenseMeLEDMatrix.setCursor(x,0);
      SenseMeLEDMatrix.print(msg);
      SenseMeLEDMatrix.writeDisplay();
    delay(100);
  }
	
}



SenseMeLEDMatrixClass SenseMeLEDMatrix = SenseMeLEDMatrixClass();