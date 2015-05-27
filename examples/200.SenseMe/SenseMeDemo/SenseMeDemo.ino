#include <SenseMeThermistor.h>
#include <Wire.h>
#include <SenseMeAccelerometer.h>
#include <SenseMeLEDMatrix.h>

//#include <Adafruit_NeoPixel.h>
#include <SenseMeNeopixel.h>
#include <SHT2x.h>
/*
 SenseMeDemo

 Thie is a SinceMeDemo utilises
  - LED Matrix
  - Bluetooth
  - NeoPixel - In the cube
  - Random number
  - Accelerometer
 If you put the SenseMe cube with the transparent side on the top, it reads temperature and display the reading in Celcius
 If you put the SenseMe cube with cube side on the top, it display a random face (smiley, frown or neutral)
 If you put tte SenseMe cube with Bluetooth side on the top, it becomes a bluetooth serial receiver and display the received ASCII on the LED matrix
 Author: Venus SHum
*/



//Adafruit_NeoPixel Neopixel = Adafruit_NeoPixel(NUMNeopixel, PIN, NEO_GRB + NEO_KHZ800);



boolean setNum = false;

//Bluetooth
String content = "";
//char * content;
char character;
char incomingByte;  // incoming data

boolean DEBUG = false;
uint8_t brightness=0, dir=0;
uint8_t state=0, r=0, g=0, b=255;

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  blue_bmp[] =
  { B00011000,
    B10010100,
    B01010010,
    B00110100,
    B00111000,
    B01010100,
    B10010010,
    B00011100 },
  thermo_bmp[] =
  { B00111000,
    B00101110,
    B00101000,
    B00101110,
    B00101000,
    B01000100,
    B01000100,
    B00111000 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 };
    
void setup()
{
  Serial.begin(115200);
  //SenseMeThermistor.begin();
  
  SenseMeLEDMatrix.begin();  // pass in the address
  SenseMeAccelerometer.begin();
  SenseMeLEDMatrix.clear();
  SenseMeLEDMatrix.writeDisplay();
  randomSeed(analogRead(0));
  setNum = false;
  Serial1.begin(9600); // initialization
  //Serial.begin(9600);
  SenseMeNeopixel.begin();
  SenseMeNeopixel.setColor(0,0,0); 
  
}

long ran;

void loop()
{   
    float xyz[3];
    SenseMeAccelerometer.xyz(xyz);
    if (DEBUG) {
      Serial.print("Accelerometer x=");
      Serial.print(xyz[0]);
      Serial.print(", y=");
      Serial.print(xyz[1]);
      Serial.print(", z=");
      Serial.println(xyz[2]);
    }
    
    
    if (xyz[0] > 0.2 || xyz[0] < -0.2) {
      //mode 1
      SenseMeLEDMatrix.setRotation(0);

        SenseMeLEDMatrix.clear(); 
        SenseMeLEDMatrix.drawBitmap(0, 0, thermo_bmp, 8, 8, LED_ON);
        SenseMeLEDMatrix.writeDisplay();
        pulseColor(0,255,0);
        tempLight();
        //delay(1000);

      
      setNum = false;
    }
    else if (xyz[1] > 0.2 || xyz[1] < -0.2) {
      //mode 2
      //demoMatrix();
      SenseMeLEDMatrix.setRotation(2);
      if (setNum==false) {
         ran = random(3);
         setNum = true;
         Serial.println(ran);
     }
     luckyFace(ran);
      //setNum = false;
    }
    else if (xyz[2] > 0.2 || xyz[2] < -0.2) {
     //mode 3
     /*
     if (setNum==false) {
         long ran = random(3);
         luckyFace(ran);
         setNum = true;
         Serial.println(ran);
     }
     */
     //matrix.clear();
     SenseMeLEDMatrix.setRotation(3);  //Bluetooth symbol on the top, the matrix is now facing this way. 
     SenseMeLEDMatrix.drawBitmap(0, 0, blue_bmp, 8, 8, LED_ON);
     SenseMeLEDMatrix.writeDisplay();
     //delay(1000);
     pulseColor(0,0,255); 
      //matrix.clear();
     //matrix.writeDisplay();
     Bluetooth();
     setNum = false;
    }
    
    //tempLight();
    
    //
}


void setColor() {
    SenseMeNeopixel.setBrightness(brightness),
    SenseMeNeopixel.setColor(r,g,b); // Moderately bright green color.
    SenseMeNeopixel.show(); // This sends the updated pixel color to the hardware.
    if (dir == 0)   {
        if (brightness<255) brightness++;
        else 
          dir = 1;
    }
    else {
        if (brightness > 0)
          brightness --;
        else {
          dir = 0;
          switch(state) {
            case 0:
              state = 1; r=0; g=255; b=0; break;
            case 1:
              state = 2; r=255; g=0; b=0; break;
            case 2:
              state = 3; r=255; g=255; b=255; break;
            case 3:
              state = 0; r=0; g=0; b=255; break;
          }
    }
    }
}


