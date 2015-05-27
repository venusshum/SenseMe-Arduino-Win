/**
* \defgroup SenseMeLightSensor Driver for SenseMe Light Sensor
* 
* @{
*/

/**
* \file 
* 		SenseMe Light sensor driver
* \author
* 		Venus Shum/support@SenseMe.org
*/

#ifndef __SENSEMELIGHTSENSOR_H__
#define __SENSEMELIGHTSENSOR_H__

#include <stdio.h>
#include <Arduino.h>
#include <SenseMe.h>



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
  { 000000001,
    B00000011,
    B00011100,
    B00001000,
    B00011100,
    B11100000,
    B01000000,
    B11100000 }
      ;
	  

/***********************************************************************************************\
* Public prototypes
\***********************************************************************************************/

class SenseMeLightSensorClass 
{
	private:
	
	public:
		SenseMeLightSensorClass();
		void    begin();
		void    end();
		uint16_t lightLevel();

};

extern SenseMeLightSensorClass SenseMeLightSensor;

#endif

/** @} */
