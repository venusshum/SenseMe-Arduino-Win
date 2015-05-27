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

#ifndef __SENSEMETHERMISTOR_H__
#define __SENSEMETHERMISTOR_H__

#include <Arduino.h>
#include <SenseMe.h>
#include <NTC.h>


NTCClass SenseMeThermistor = NTCClass();

#define Thermistor SenseMeThermistor

#endif

/** @} */
