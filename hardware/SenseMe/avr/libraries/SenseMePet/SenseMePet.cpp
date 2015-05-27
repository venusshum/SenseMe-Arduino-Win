#include <SenseMePet.h>
#include <SenseMeAccelerometer.h>
int total, threshold1, threshold2;
volatile int status; // 0=happy 1=sad 2=tired 3=asleep

SenseMePetClass::SenseMePetClass()
{}

void SenseMePetClass::begin(int happy, int sad, int tired)
{
	total = happy+sad+tired;
	threshold1 = happy;
	threshold2 = happy+sad;
	status = 0;
	SenseMeAccelerometer.begin();
}

void SenseMePetClass::update()
{
	if(status == 0)//happy
	{
		long randNum = random(total);
		if (randNum>threshold2)
		{
			status = 2; //tired
		}
		else if (randNum>threshold1)
		{
			status = 1; //sad
		}
	}
	if(status == 1)//sad
	{
		delay(10);
		float magnitude = SenseMeAccelerometer.magnitude();
		//sqrt(pow(SenseMeAccelerometer.getX(),2)+pow(SenseMeAccelerometer.getY(),2)+pow(SenseMeAccelerometer.getZ(),2))
		if(magnitude>1.5)
		{
			status = 0;
		}
	}
	if(status == 2)//tired
	{
		if(SenseMeAccelerometer.getY()<(-0.9))
		{
			status = 3;
		}
	}
	if(status == 3)//asleep
	{
		if(SenseMeAccelerometer.getX()<(-0.9))
		{
			status = 0;
		}
	}
}

String SenseMePetClass::getStatus()
{
	String currentStatus = "0";
	switch(status)
	{
		case 0:
		currentStatus = "happy";
		break;
		case 1:
		currentStatus = "sad";
		break;
		case 2:
		currentStatus = "tired";
		break;
		case 3:
		currentStatus = "asleep";
		break;
	}
	return currentStatus;
}

void SenseMePetClass::setStatus(String newStatus) //expected arguments "happy" "sad" "tired" "asleep"
{
	if(newStatus == "happy")
	{
		status = 0;
	}
	else if (newStatus == "sad")
	{
		status = 1;
	}
	else if (newStatus == "tired")
	{
		status = 2;
	}
	else if (newStatus == "asleep")
	{
		status = 3;
	}
}

SenseMePetClass SenseMePet = SenseMePetClass();