#ifndef __SENSEMEPET_H__
#define __SENSEMEPET_H__

#include <Arduino.h>
#include <SenseMeAccelerometer.h>

class SenseMePetClass 
{	
	public:
		SenseMePetClass();
		void begin(int happy, int sad, int tired);
		void update();
		void setStatus(String newStatus);
		String getStatus();
};

extern SenseMePetClass SenseMePet;

#endif