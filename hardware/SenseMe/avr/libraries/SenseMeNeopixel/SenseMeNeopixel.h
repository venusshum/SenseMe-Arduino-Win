#include <Adafruit_NeoPixel.h>


enum colour {
	RED,
	GREEN,
	BLUE,
	YELLOW,
	MAGENTA,
	CYAN,
	WHITE,
	OFF
};

class SenseMeNeopixelClass : public Adafruit_NeoPixel
{
	private:
	
	public:
	SenseMeNeopixelClass();
	void begin();
	void setColor(uint8_t r, uint8_t g, uint8_t b);
	//void setColor(uint32_t c);
	void setColor(colour c);
	void setColor(colour c, uint8_t brightness);
	void setColor(uint8_t r, uint8_t g, uint8_t b,uint8_t brightness);

};

extern SenseMeNeopixelClass SenseMeNeopixel;



#define Pixel SenseMeNeopixel
//#define Neopixel SenseMeNeopixel