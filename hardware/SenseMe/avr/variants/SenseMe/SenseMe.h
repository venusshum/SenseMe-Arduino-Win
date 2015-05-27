/*
 * SenseMe.h
 * SenseMe V1
 * 
 * Vesrion	  Date			By		Comment
 * 
 * 1.0.0	  13.04.2015	VS		Initial release.
 *
 * \file
 *        Configuration file for the SenseMe board.
 *
 * \author
 *	Venus Shum codeme.io
 */

#ifndef __SENSEME_H__
#define __SENSEME_H__

#define  __SENSEME_V1

// User LED
#define LED1   		13

// User Button
//#define BUTTON  	 7



// Data connection to the RGB LEDs
//#define LED_MOSI	16
//#define LED_MISO	14
//#define LED_SCLK	15
//#define LED_LATCH	12


// NTC
//#define NTC  		 A1


// Accelerometer connection 
#define ACC_SDA  	 2
#define ACC_SCL  	 3

// Accelerometer connection 
#define _SDA  	 2
#define _SCL  	 3


// External I2C connection 
#define I2C_SDA  	 2
#define I2C_SCL  	 3

//Neopixel parameters
//#define PIXELPIN            10  //pin for NeoPixel
#define PIXELPIN            12  //pin for NeoPixel
#define NUMPIXELS      1  // How many NeoPixel are attached to the Arduino?

#define SPEAKER_PIN  11

// Light sensor 


// Bluetooth control pins
#define BTCMDPIN	5
#define BTSTATEPIN	9
#define BTRESETPIN	4

#define BUTTON A1
#define PULSESENSOR A0
//#define LIGHTSENSOR	A1		//version 0.1
#define LIGHTSENSOR	A2			//version 0.2

#endif
