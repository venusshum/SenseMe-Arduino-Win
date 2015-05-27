/**
* \defgroup SenseMeThermistor Thermistor Driver 
* 
* @{
*/

/**
* \file 
* 		Thermistor driver
* \author
* 		
*/

#ifndef __NTC_H__
#define __NTC_H__

#include <Arduino.h>
#include <SenseMe.h>

enum temperatureUnits {
  CELSIUS,
  KELVIN,
  FAHRENHEIT
};

class NTCClass 
{
	private:

	public:
		NTCClass();
		void     begin();
		void     end();
		float    temperature(temperatureUnits units=CELSIUS);
		float    temperature(temperatureUnits units, float B, float T0, float R0, float R_Balance);
		uint16_t temperatureRaw();
};


#endif

/** @} */
