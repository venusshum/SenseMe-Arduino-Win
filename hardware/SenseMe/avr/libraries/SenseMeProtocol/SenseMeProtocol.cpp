/**
* \addtogroup SenseMeButton
*
* This is the driver code for the button on the SenseMe. This
* uses an ISR to debounce the button - it uses edges rather than
* the value of the digital pin.
*
* The debounce time can be set in the begin routine, but defaults
* to the DEBOUNCE_DELAY value defined in the header.
*
* @{
*/

/**
* \file
* 		SenseMe Button driver
* \author
* 		codeme.io
*/

#include "pins_arduino.h"
#include "SenseMeProtocol.h"

#define DEBUGG 0
#if DEBUGG
	#define PRINTLN(...) Serial.println(__VA_ARGS__)
	#define PRINT(...) Serial.print(__VA_ARGS__)
#else if
	#define PRINTLN(...)
	#define PRINT(...)
#endif

#define LED1 13
/*---------------------------------------------------------------------------*/
/**
* \brief Constructor
*
* C++ constructor for this class. Empty.
*/
SenseMeProtocolClass::SenseMeProtocolClass()
{
}

/*---------------------------------------------------------------------------*/
/**
* \brief Destructor
*
* C++ destructor for this class. Free memory.
*/
SenseMeProtocolClass::~SenseMeProtocolClass()
{
}

/*---------------------------------------------------------------------------*/
/**
* \brief begin function - must be called before using other functions
*
* This function enables the button. It can be called with a parameter that
* represents the debounce delay, but this defaults to a sensible value if it
* is not provided.
*
*/
void SenseMeProtocolClass::begin(SenseMeLEDsClass *eledsc,
								  SenseMeAccelerometerClass *eaccc,
							      SenseMeThermistorClass *etc,
							      SenseMeMagnetometerClass *emc,
							      SenseMeLightClass *elc,
							      SenseMeButtonClass *ebc,
								  SenseMeIRClass *eirc)
{
	PRINTLN("PROTOCOL begin");
	commChannel = -1;

	pinMode(LED1, OUTPUT); // Small green LED

	SenseMeLEDs = eledsc;
	SenseMeAccelerometer = eaccc;
	SenseMeThermistor = etc;
	SenseMeMagnetometer = emc;
	SenseMeLight = elc;
	SenseMeButton = ebc;
	SenseMeIR = eirc;

	// Timer 1 inicialization
	cli();	// Disable interrupts
    TCCR1B |= 1<<CS10 | 1<<CS11 | 1<<WGM12; // set up timer with prescaler = 64 and CTC mode
	TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
    TCNT1 = 0; 		// initialize counter
    OCR1A = 1000; 	// initialize compare value (no difference..!?)
    sei();	// Enable interrupts

	loadDefaults();
}

/*---------------------------------------------------------------------------*/
/**
* \brief end function - switch off the button
*
* Switch off the interrupt and reset state.
*
*/
void SenseMeProtocolClass::end()
{
}

/*---------------------------------------------------------------------------*/
/**
* \brief bla
*
* Load default settings.
*
*/
void SenseMeProtocolClass::loadDefaults()
{
	overSampling = 0; // Number of samples per second

	for(byte i = 0; i < NR_SENSORS; i++)
	{
		sensorsSamples[i] = 0;
	}

	for(byte i = 0; i < NR_SENSORS_BUF; i++)
	{
		sensorsSum[i] = 0;
	}

	overSamplingCnt = 0;
	overSamplingMatch = 0;
	overSamplingRead = 0;

	for(byte i = 0; i < NR_SENSORS; i++)
	{
		sensorsMatch[i] = -1;
		sensorsCnt[i] = 0;
		sensorsReadOnce[0] = 0;
	}

	buttonCatchEnable[BUTTON_PRESSED] = false;
	buttonCatchEnable[BUTTON_RELEASED] = false;

	for(byte i = 0; i < IRBUFSZ; i++)
	{
		irBuf[i] = 0;
	}

	cntt = 0;
}

// ======================= PACKAGE =========================//
/*---------------------------------------------------------------------------*/
/**
* \brief bla
*
* bla bla
*
*/
int SenseMeProtocolClass::parsePackage(String str, byte *outNrVals, long *outVals)
{
	byte cnt = 0;
	int indx = 0;
	int oldIndx = 0;
	PRINTLN("Package received: " + str);
	String sval;
	str.concat(PACKAGE_DELIMITER_CHR); // Add final delimiter
	for(int i = 0; i < PACKAGE_MAXNR_VALUES; i++)
	{
		indx = str.indexOf(PACKAGE_DELIMITER_CHR, oldIndx);
		if(indx == -1) break; // No more strings
		sval = str.substring(oldIndx, indx);
		oldIndx = indx+1;
		outVals[i] = sval.toInt();
		cnt++;
	}
	*outNrVals = cnt;
	PRINTLN(cnt);
	return RES_OK;
}

/*---------------------------------------------------------------------------*/
/**
* \brief bla
*
* bla bla
*
*/
int SenseMeProtocolClass::parsePackageHeader(byte communicationChannel, byte inNrVals, long *inVals, struct SenseMePackage *outPackage, byte *outHeaderSize)
{
	commChannel = communicationChannel;
	if(inNrVals > 0)
	{
		switch (inVals[PACKAGE_PACKAGE_TYPE])
		{
			case PACKAGE_TYPE_1:
				if(inNrVals < 2) return RES_ERR_PACKAGE_HEADER;
				outPackage->packageType = PACKAGE_TYPE_1;
				outPackage->commandID = (byte)inVals[1];
				*outHeaderSize = 2;
				break;
			case PACKAGE_TYPE_2:
				if(inNrVals < 4) return RES_ERR_PACKAGE_HEADER;
				outPackage->packageType = PACKAGE_TYPE_1;
				outPackage->commandID = (byte)inVals[1];
				outPackage->packageID = (long)inVals[2];
				outPackage->ack = 		(byte)inVals[3];
				*outHeaderSize = 4;
				break;
			default:
				return RES_ERR_PACKAGE_HEADER;
		}
	}
	else
	{
		return RES_ERR_PACKAGE_HEADER;
	}

	return RES_OK;
}

/*---------------------------------------------------------------------------*/
/**
* \brief Send package on Serial port
*
* Format: {a,b,c,...x,y,z}
*
*/
int SenseMeProtocolClass::sendPackage(byte inNrVals, byte inCommandID, long *inVals)
{
	if(commChannel == COMMUNICATION_PC_TERMINAL)	// COMMUNICATION_PC_TERMINAL
	{
		PRINTLN("sendPackage: COMMUNICATION_PC_TERMINAL");
		Serial.print(PACKAGE_START_CHR);
		Serial.print(PACKAGE_TYPE_1);
		Serial.print(PACKAGE_DELIMITER_CHR);
		Serial.print(inCommandID);
		for(byte i=0; i<inNrVals; i++)
		{
			Serial.print(PACKAGE_DELIMITER_CHR);
			Serial.print(inVals[i]);
		}
		Serial.print(PACKAGE_STOP_CHR);
	}
	else if(commChannel == COMMUNICATION_BT_MODULE)	// COMMUNICATION_BT_MODULE
	{
		PRINTLN("sendPackage: COMMUNICATION_BT_MODULE");
		Serial1.print(PACKAGE_START_CHR);
		Serial1.print(PACKAGE_TYPE_1);
		Serial1.print(PACKAGE_DELIMITER_CHR);
		Serial1.print(inCommandID);
		for(byte i=0; i<inNrVals; i++)
		{
			Serial1.print(PACKAGE_DELIMITER_CHR);
			Serial1.print(inVals[i]);
		}
		Serial1.print(PACKAGE_STOP_CHR);
	}
	else
	{
		return RES_ERR;
	}
	return RES_OK;
}

int SenseMeProtocolClass::sendPackage(struct SenseMePackage *engPackage, byte inNrVals, byte inCommandID, long *inVals)
{
	if(commChannel == COMMUNICATION_PC_TERMINAL) // COMMUNICATION_PC_TERMINAL
	{
		PRINTLN("sendPackage: COMMUNICATION_PC_TERMINAL");
		Serial.print(PACKAGE_START_CHR);
		switch (engPackage->packageType)
		{
			case PACKAGE_TYPE_1:
				Serial.print(engPackage->packageType);
				Serial.print(PACKAGE_DELIMITER_CHR);
				Serial.print(inCommandID);
				break;
			case PACKAGE_TYPE_2:
				Serial.print(engPackage->packageType);
				Serial.print(PACKAGE_DELIMITER_CHR);
				Serial.print(inCommandID);
				Serial.print(PACKAGE_DELIMITER_CHR);
				Serial.print(engPackage->packageID);
				Serial.print(PACKAGE_DELIMITER_CHR);
				Serial.print(engPackage->ack);
				break;
			default:
				return RES_ERR_PACKAGE_HEADER;
		}

		for(byte i=0; i<inNrVals; i++)
		{
			Serial.print(PACKAGE_DELIMITER_CHR);
			Serial.print(inVals[i]);
		}
		Serial.print(PACKAGE_STOP_CHR);
	}
	else if(commChannel == COMMUNICATION_BT_MODULE) // COMMUNICATION_BT_MODULE
	{
		PRINTLN("sendPackage: COMMUNICATION_BT_MODULE");
		Serial1.print(PACKAGE_START_CHR);
		switch (engPackage->packageType)
		{
			case PACKAGE_TYPE_1:
				Serial1.print(engPackage->packageType);
				Serial1.print(PACKAGE_DELIMITER_CHR);
				Serial1.print(inCommandID);
				break;
			case PACKAGE_TYPE_2:
				Serial1.print(engPackage->packageType);
				Serial1.print(PACKAGE_DELIMITER_CHR);
				Serial1.print(inCommandID);
				Serial1.print(PACKAGE_DELIMITER_CHR);
				Serial1.print(engPackage->packageID);
				Serial1.print(PACKAGE_DELIMITER_CHR);
				Serial1.print(engPackage->ack);
				break;
			default:
				return RES_ERR_PACKAGE_HEADER;
		}

		for(byte i=0; i<inNrVals; i++)
		{
			Serial1.print(PACKAGE_DELIMITER_CHR);
			Serial1.print(inVals[i]);
		}
		Serial1.print(PACKAGE_STOP_CHR);
	}
	else
	{
		return RES_ERR;
	}
	return RES_OK;
}


// ======================= COMMANDS ========================//
/*---------------------------------------------------------------------------*/
/**
* \brief Small green LED
*
* bla bla
*
*/
int SenseMeProtocolClass::setLED(struct SenseMePackage *engPackage, byte nrVals, long *inVals)
{
	if(nrVals == 1)
	{
		(inVals[0]) ? digitalWrite(LED1, HIGH) : digitalWrite(LED1, LOW);
		return RES_OK;
	}
	else
	{
		return RES_ERR_PACKAGE_NR_VALS;
	}
}

/*---------------------------------------------------------------------------*/
/**
* \brief RGB LEDs
*
* bla bla
*
*/
int SenseMeProtocolClass::setLEDs(struct SenseMePackage *engPackage, byte nrVals, long *inVals)
{
	if(nrVals == 1)		// Set colour on All LEDs
	{
		SenseMeLEDs->setAll((colour)inVals[0]);
		return RES_OK;
	}
	if(nrVals == 2)		// Set colour on one LED
	{
		SenseMeLEDs->setLED((uint8_t)inVals[0], (colour)inVals[1]);
		return RES_OK;
	}
	else if(nrVals == 3)		// Set colour and brightness on one LED
	{
		SenseMeLEDs->setLED((uint8_t)inVals[0], (colour)inVals[1], (uint8_t)inVals[2]);
		return RES_OK;
	}
	else if(nrVals == 4)		// Set RGB colour on one LED
	{
		SenseMeLEDs->setLED((uint8_t)inVals[0], (uint8_t)inVals[1], (uint8_t)inVals[2], (uint8_t)inVals[3]);
		return RES_OK;
	}
	else if(nrVals == 16)		// Set only colours
	{
		for(int i=0; i<16; i++) ledsColor[i] = (colour)inVals[i];
		SenseMeLEDs->setLEDs(ledsColor);
		return RES_OK;
	}
	else if(nrVals == 32)	// Set colours and brightness
	{
		for(int i = 0; i < 16; i++)
		{
			ledsColor[i] = (colour)inVals[i];
			ledsBrightness[i] = (uint8_t)inVals[i+16];
		}
		SenseMeLEDs->setLEDs(ledsColor, ledsBrightness);
		return RES_OK;
	}
	else if(nrVals == 48)	// Set rgb value for each LED. Format [r0, g0, b0, ... , r15, g15, b15]
	{
		for(int i = 0; i < 16; i++)
		{
			ledsRGB[0][i] = (uint8_t)inVals[(i*3) + 0]; // Red
			ledsRGB[1][i] = (uint8_t)inVals[(i*3) + 1]; // Green
			ledsRGB[2][i] = (uint8_t)inVals[(i*3) + 2]; // Blue
		}
		SenseMeLEDs->setLEDs(ledsRGB);
		return RES_OK;
	}
	else
	{
		return RES_ERR_PACKAGE_NR_VALS;
	}
}

/*---------------------------------------------------------------------------*/
/**
* \brief set or get
*
* setGet = 0 -> set
* setGet = 1 -> get
*
* inVals represents letters (ASCII values; http://www.asciitable.com/) separated by delimiter.
* E.g.: inVals = [69,110,103,100,117,105,110,111] -> "SenseMe"
*/
int SenseMeProtocolClass::setGetIR(struct SenseMePackage *engPackage, byte nrVals, long *inVals, uint8_t setGet)
{
	int j = 0;;
	if(setGet == 0) // Set
	{
		if(nrVals < IRBUFSZ)
		{
			for(byte i = 0; i < nrVals; i++)
			{
				irBuf[j++] = (uint8_t)(inVals[i]);
			}
			irBuf[j++] = 0;
			SenseMeIR->send(irBuf, j);
			return RES_OK;
		}
		else
		{
			return RES_ERR_PACKAGE_NR_VALS;
		}

	}
	else	// Get
	{
		int len = 0;
		uint8_t  buf[IRBUFSZ];

		if(nrVals == 1)
		{
			// Wait and read the message, if there are some data. Blocking code!
			len = SenseMeIR.recv(buf, (uint16_t)(inVals[0]));

			if (len < 0) len = 0;
			for(byte i = 0; i < len; i++)
			{
				vals[i] = (long)(buf[i]);
			}
			sendPackage(engPackage, len, COM_GET_IR, vals);
			return RES_OK;
		}
		else
		{
			return RES_ERR_PACKAGE_NR_VALS;
		}
	}
	return RES_OK;
}

/*---------------------------------------------------------------------------*/
/**
* \brief set or get
*
* setGet = 0 -> set
* setGet = 1 -> get
*
* All parameters must be specified as KEY-VALUE pair.
* E.g.: inVals = [0,12, 1,23, 6,0, ...]
*/
int SenseMeProtocolClass::setGetStatus(struct SenseMePackage *engPackage, byte nrVals, long *inVals, uint8_t setGet)
{
	int nrOf;
	if(setGet == 0) // Set
	{
		// Number of input parameters must be even.
		if(nrVals % 2) return RES_ERR_PACKAGE_NR_VALS;
		nrOf = nrVals/2;
		for(int i = 0; i < nrOf; i++)
		{
			switch (inVals[i*2]) // Key
			{
				case STATUS_OVERSAMPLING: // Number of samples per second
					overSampling = (uint16_t)inVals[(i*2)+1]; // Value
					if ((uint16_t)(overSampling * TIME_STAMP) > 1000) overSampling = (uint16_t)(1000 / TIME_STAMP);
					overSamplingMatch = (uint16_t)(1000 / (overSampling * TIME_STAMP));
					for(byte j = 0; j < NR_SENSORS_BUF; j++) sensorsSum[j] = 0;
					for(byte j = 0; j < NR_SENSORS; j++) sensorsSamples[j] = 0;
					overSamplingCnt = 0;
					overSamplingRead = 0;
					break;
				default:
					return RES_ERR_PACKAGE_UNKNOWN_STATUS_KEY;
			}
		}
	}
	else	// Get
	{
		nrOf = nrVals;
		for(int i = 0; i < nrOf; i++)
		{
			switch (inVals[i]) // Key
			{
				case STATUS_OVERSAMPLING: // Number of samples per second
					vals[(i*2)+0] = (long)STATUS_OVERSAMPLING;	// Key
					vals[(i*2)+1] = (long)overSampling;			// Value
					break;
				default:
					return RES_ERR_PACKAGE_UNKNOWN_STATUS_KEY;
			}
		}
		sendPackage(engPackage, nrOf*2, COM_GET_STATUS, vals);
	}
	return RES_OK;
}

/*---------------------------------------------------------------------------*/
/**
* \brief Version
*
* Sends program version which depends on board and running sketch on it.
*
*/
int SenseMeProtocolClass::getVersion(struct SenseMePackage *engPackage)
{
	vals[0] = VERSION;
	sendPackage(engPackage, 1, COM_GET_VERSION, vals);
	return RES_OK;
}

/*---------------------------------------------------------------------------*/
/**
* \brief Sensors
*
* [x], In the case of no parameter read once
*
* x:
*	-1 -> stop
*	 0 -> read once
*	>0 -> continuous reading interval [ms]
*/
int SenseMeProtocolClass::getSensor(struct SenseMePackage *engPackage, int sensorType, byte nrVals, long *inVals)
{
	if(nrVals > 1) return RES_ERR_PACKAGE_NR_VALS;
	if(sensorType >= NR_SENSORS) return RES_ERR_PACKAGE_UNKNOWN_SENSOR_TYPE;
	if(nrVals == 1 && inVals[0] == -1) // stop reading
	{
		sensorsMatch[sensorType] = -1;
		return RES_OK;
	}
	else if(nrVals == 1 && inVals[0] > 0) // set continuous reading interval
	{
		if(inVals[0] < MIN_TIME) inVals[0] = MIN_TIME;
		sensorsMatch[sensorType] = (long)(inVals[0] / TIME_STAMP);
		sensorsCnt[sensorType] = 0;
	}
	else
	{
		sensorsMatch[sensorType] = 1000000000L;
		sensorsCnt[sensorType] = 0;
	}

	// Read value in the main loop.
	sensorsReadOnce[sensorType] = 1;

	return RES_OK;
}

/*---------------------------------------------------------------------------*/
/**
* \brief Button
*
* [x], In the case of no parameter read once
*
* x:
*	-1 -> stop
*	 0 -> read once
*	 1 -> onPressed
*    2 -> onReleased
*    3 -> onBoth
*/
int SenseMeProtocolClass::getButton(struct SenseMePackage *engPackage, byte nrVals, long *inVals)
{
	if(nrVals > 1) return RES_ERR_PACKAGE_NR_VALS;
	if(nrVals == 1 && inVals[0] != 0)
	{
		switch(inVals[0])
		{
			case -1:
				buttonCatchEnable[BUTTON_PRESSED] = false;
				buttonCatchEnable[BUTTON_RELEASED] = false;
				break;
			case 1:
				buttonCatchEnable[BUTTON_PRESSED] = true;
				break;
			case 2:
				buttonCatchEnable[BUTTON_RELEASED] = true;
				break;
			case 3:
				buttonCatchEnable[BUTTON_PRESSED] = true;
				buttonCatchEnable[BUTTON_RELEASED] = true;
				break;
			default:
				return RES_ERR_PACKAGE_VAL;
		}
		return RES_OK;
	}

	// Read once
	(SenseMeButton->isPressed()) ? vals[0] = 1 : vals[0] = 0;
	sendPackage(engPackage, 1, COM_GET_BUTTON, vals);
	return RES_OK;
}


// Intern functions
void SenseMeProtocolClass::_readThermistor(long *outVals)
{
	outVals[0] = (long)(SenseMeThermistor->temperature()*1000.0); // Convert to mili °C
}

void SenseMeProtocolClass::_readAccelerometer(long *outVals)
{
	float xyz[3];
	SenseMeAccelerometer->xyz(xyz);
	for(byte i=0; i<3; i++) outVals[i] = (long)(xyz[i]*1000.0); // Convert to mili G
	//Serial.print(outVals[0]);
	//Serial.print(" ");
	//Serial.print(outVals[1]);
	//Serial.print(" ");
	//Serial.print(outVals[2]);
	//Serial.println(" ");
}

void SenseMeProtocolClass::_readMagnetometer(long *outVals)
{
	float xyz[3];
	SenseMeMagnetometer->xyz(xyz);
	for(byte i=0; i<3; i++) outVals[i] = (long)(xyz[i]); //The values range from -20000 to +20000 counts
}

void SenseMeProtocolClass::_readLight(long *outVals)
{
	outVals[0] = (long)(SenseMeLight->lightLevel()); // A 10 bit value - so in a range 0-1023
	PRINT("SENSOR_LIGHT: ");
	PRINTLN(outVals[0]);
}

void SenseMeProtocolClass::_readSensors(long *outVals)
{
	int idx = 0;
	_readThermistor(&outVals[idx]); idx = idx + 1;
	_readAccelerometer(&outVals[idx]); idx = idx + 3;
	_readMagnetometer(&outVals[idx]); idx = idx + 3;
	_readLight(&outVals[idx]); idx = idx + 1;
}

void SenseMeProtocolClass::mainLoop()
{
	// Oversampling read
	if(overSamplingRead == 1)
	{
		for(byte i = 0; i < NR_SENSORS; i++)
		{
			if(sensorsMatch[i] > 0)
			{
				switch(i)
				{
					case SENSOR_TEMP:
						PRINTLN("OS_SENSOR_TEMP ");
						_readThermistor(vals);
						sensorsSum[SENSOR_TEMP_BUF] += vals[0];
						sensorsSamples[SENSOR_TEMP]++;
						break;
					case SENSOR_ACC:
						PRINTLN("OS_SENSOR_ACC");
						_readAccelerometer(vals);
						for(byte j = 0; j < 3; j++) sensorsSum[SENSOR_ACC_X_BUF + j] += vals[j];
						sensorsSamples[SENSOR_ACC]++;
						break;
					case SENSOR_MAG:
						PRINTLN("OS_SENSOR_MAG");
						_readMagnetometer(vals);
						for(byte j = 0; j < 3; j++) sensorsSum[SENSOR_MAG_X_BUF + j] += vals[j];
						sensorsSamples[SENSOR_MAG]++;
						break;
					case SENSOR_LIGHT:
						PRINTLN("OS_SENSOR_LIGHT");
						_readLight(vals);
						sensorsSum[SENSOR_LIGHT_BUF] += vals[0];
						sensorsSamples[SENSOR_LIGHT]++;
						break;
					case SENSOR_ALL:
						PRINTLN("OS_SENSOR_ALL");
						_readSensors(vals);
						for(byte j = 0; j < NR_SENSORS_BUF; j++) sensorsSum[j] += vals[j];
						sensorsSamples[SENSOR_ALL]++;
						break;
				}
			}
		}
		overSamplingRead = 0;
	}

	for(byte i = 0; i < NR_SENSORS; i++)
	{
		if(((sensorsMatch[i] > 0) && (sensorsCnt[i] >= sensorsMatch[i])) || sensorsReadOnce[i] == 1)
		{
			sensorsCnt[i] = 0;
			switch(i)
			{
				case SENSOR_TEMP:
					PRINTLN("SENSOR_TEMP ");
					if(overSampling == 0 || sensorsSamples[SENSOR_TEMP] == 0)
					{
						_readThermistor(vals);
						vals[1] = 1; // One sample taken.
					}
					else
					{
						vals[0] = sensorsSum[SENSOR_TEMP_BUF] / sensorsSamples[SENSOR_TEMP];
						vals[1] = sensorsSamples[SENSOR_TEMP];
						sensorsSum[SENSOR_TEMP_BUF] = 0;
						sensorsSamples[SENSOR_TEMP] = 0;
					}
					SenseMeProtocol.sendPackage(2, COM_GET_TEMPERATURE, vals);
					break;

				case SENSOR_ACC:
					PRINTLN("SENSOR_ACC");
					if(overSampling == 0 || sensorsSamples[SENSOR_ACC] == 0)
					{
						_readAccelerometer(vals);
						vals[3] = 1;
					}
					else
					{
						for(byte j = 0; j < 3; j++)
						{
							vals[j] = sensorsSum[SENSOR_ACC_X_BUF + j] / sensorsSamples[SENSOR_ACC];
							sensorsSum[SENSOR_ACC_X_BUF + j] = 0;
						}
						vals[3] = sensorsSamples[SENSOR_ACC];
						sensorsSamples[SENSOR_ACC] = 0;
					}
					SenseMeProtocol.sendPackage(4, COM_GET_ACCELEROMETER, vals);
					break;

				case SENSOR_MAG:
					PRINTLN("SENSOR_MAG");
					if(overSampling == 0 || sensorsSamples[SENSOR_MAG] == 0)
					{
						_readMagnetometer(vals);
						vals[3] = 1;
					}
					else
					{
						for(byte j = 0; j < 3; j++)
						{
							vals[j] = sensorsSum[SENSOR_MAG_X_BUF + j] / sensorsSamples[SENSOR_MAG];
							sensorsSum[SENSOR_MAG_X_BUF + j] = 0;
						}
						vals[3] = sensorsSamples[SENSOR_MAG];
						sensorsSamples[SENSOR_MAG] = 0;
					}
					SenseMeProtocol.sendPackage(4, COM_GET_MAGNETOMETER, vals);
					break;

				case SENSOR_LIGHT:
					PRINTLN("SENSOR_LIGHT");
					if(overSampling == 0 || sensorsSamples[SENSOR_LIGHT] == 0)
					{
						_readLight(vals);
						vals[1] = 1;
					}
					else
					{
						vals[0] = sensorsSum[SENSOR_LIGHT_BUF] / sensorsSamples[SENSOR_LIGHT];
						vals[1] = sensorsSamples[SENSOR_LIGHT];
						sensorsSum[SENSOR_LIGHT_BUF] = 0;
						sensorsSamples[SENSOR_LIGHT] = 0;
					}
					SenseMeProtocol.sendPackage(2, COM_GET_LIGHT, vals);
					break;

				case SENSOR_ALL:
					PRINTLN("SENSOR_ALL");
					if(overSampling == 0 || sensorsSamples[SENSOR_ALL] == 0)
					{
						_readSensors(vals);
						vals[8] = 1;
					}
					else
					{
						for(byte j = 0; j < NR_SENSORS_BUF; j++)
						{
							vals[j] = sensorsSum[j] / sensorsSamples[SENSOR_ALL];
							sensorsSum[j] = 0;
						}
						vals[8] = sensorsSamples[SENSOR_ALL];
						sensorsSamples[SENSOR_ALL] = 0;
					}
					SenseMeProtocol.sendPackage(9, COM_GET_SENSORS, vals);
					break;
			}
			sensorsReadOnce[i] = 0;
		}
	}

	// Detect button press or release
	if(buttonCatchEnable[BUTTON_PRESSED] && SenseMeButton->wasPressed())
	{
		PRINTLN("BUTTON_PRESSED");
		vals[0] = 1;
		sendPackage(1, COM_GET_BUTTON, vals);
	}
	else if(buttonCatchEnable[BUTTON_RELEASED] && SenseMeButton->wasReleased())
	{
		PRINTLN("BUTTON_RELEASED");
		vals[0] = 2;
		sendPackage(1, COM_GET_BUTTON, vals);
	}
}

/*---------------------------------------------------------------------------*/
/**
* \brief Timer1 interrupt
*
* 500Hz or 2ms
*
*/

ISR(TIMER1_COMPA_vect)
{
	SenseMeProtocol.cntt++;
	if((SenseMeProtocol.cntt % 500) == 0) PRINTLN(".");
	for(byte i = 0; i < NR_SENSORS; i++)
	{
		SenseMeProtocol.sensorsCnt[i]++;
	}

	if(SenseMeProtocol.overSampling > 0)
	{
		SenseMeProtocol.overSamplingCnt++;
		if(SenseMeProtocol.overSamplingCnt >= SenseMeProtocol.overSamplingMatch)
		{
			SenseMeProtocol.overSamplingRead = 1;
			SenseMeProtocol.overSamplingCnt = 0;
		}
	}
}

/*---------------------------------------------------------------------------*/
/*
 * Preinstantiate Objects
 */
SenseMeProtocolClass SenseMeProtocol = SenseMeProtocolClass();

/** @} */
