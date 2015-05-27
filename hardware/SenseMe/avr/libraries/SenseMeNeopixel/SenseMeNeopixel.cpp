#include <Adafruit_NeoPixel.h>
#include <SenseMe.h>
#include <SenseMeNeopixel.h>

#define LEDnum	0

SenseMeNeopixelClass::SenseMeNeopixelClass() : Adafruit_NeoPixel(1, PIXELPIN, NEO_GRB + NEO_KHZ800){
	setPixelColor(LEDnum, Color(0,0,0)); 
}


void SenseMeNeopixelClass::begin(){
	pinMode(PIXELPIN, OUTPUT);
	digitalWrite(PIXELPIN, LOW);

}


void SenseMeNeopixelClass::setColor(uint8_t r, uint8_t g, uint8_t b){
	setPixelColor(LEDnum, r,g,b);
	show();
}

/*
void SenseMeNeopixelClass::setColor(uint32_t c){
	setPixelColor(LEDnum, c);
	show();
}
*/

void SenseMeNeopixelClass::setColor(uint8_t r, uint8_t g, uint8_t b,uint8_t brightness){
	setBrightness(brightness);
	setPixelColor(LEDnum, r,g,b);
	show();
}

void SenseMeNeopixelClass::setColor(colour c){

	switch (c) {
		case RED: 
			setColor(255,0,0);
			break;
		case GREEN: 
			setColor(0,255,0);
			break;
		case BLUE: 
			setColor(0,0,255);
			break;
		case YELLOW: 
			setColor(255, 255, 0);
			break;
		case MAGENTA: 
			setColor(255,0,255);
			break;
		case CYAN:
			setColor(0,255,255);
			break;
		case WHITE:
			setColor(255,255,255);
			break;
		case OFF:
			setBrightness(0);
			break;
	}
	
	show();
}

void SenseMeNeopixelClass::setColor(colour c, uint8_t b){
	setBrightness(b);
	switch (c) {
		case RED: 
			setColor(255,0,0);
			break;
		case GREEN: 
			setColor(0,255,0);
			break;
		case BLUE: 
			setColor(0,0,255);
			break;
		case YELLOW: 
			setColor(255, 255, 0);
			break;
		case MAGENTA: 
			setColor(255,0,255);
			break;
		case CYAN:
			setColor(0,255,255);
			break;
		case WHITE:
			setColor(255,255,255);
			break;
		case OFF:
			setBrightness(0);
			break;
	}
	
	show();
}

SenseMeNeopixelClass SenseMeNeopixel = SenseMeNeopixelClass();
