/* Bluetooth HC_05 driver 
   \author: Venus Shum
*/

#include <Arduino.h>
#include <HC_05.h>

//#define DEBUG
#ifdef DEBUG
#define PRINTLN	Serial.println
#define PRINT Serial.print
#else
#define PRINTLN 
#define PRINT
#endif

//class definition
HC_05::HC_05(int cmdPin, int statePin, int resetPin)
{
    pinMode(cmdPin, OUTPUT);
    _cmdPin = cmdPin;
    cmdMode = false;

    pinMode(statePin, INPUT);
    _statePin = statePin;
	
	pinMode(resetPin, OUTPUT);
	_resetPin = resetPin;

    _bufsize = sizeof(_buffer)/sizeof(char);
}

// Begin initialisation
void HC_05::begin(void)
{
#ifdef DEBUG
    Serial.begin(9600);
#endif
	BTSERIAL.begin(38400);
}


// Print version of the BT module.
void HC_05::printVersion()
{
  sendCommand("AT+VERSION?");
}

// Set the name of the BT module.
void HC_05::setModuleName(String name)
{
  String tem = "AT+NAME=" + name;
  sendCommand(tem);
}

// Get the name of the BT module.
void HC_05::getModuleName()
{
  sendCommand("AT+NAME?");
}

// Get device address
void HC_05::getDeviceAddr()
{
  sendCommand("AT+ADDR?");
}


// Set the BT device to Master
void HC_05::setMaster()
{
  sendCommand("AT+ROLE=1");
}

// Set the BT device to Slave
void HC_05::setSlave()
{
  sendCommand("AT+ROLE=0");
}

//return a single character 
char HC_05::readChar()
{
  char ret = 0;
  if (Serial1.available())
  {
    // Forward everything to the PC terminal.
    ret = Serial1.read();
    //PRINT(ret);
  }
  return ret;
}

////////////////////
// Send AT command.
void HC_05::setCmdMode(int i_cmdMode = 1)
{
  PRINT("BtSetCmdMode ");    
  PRINTLN(i_cmdMode);    
  digitalWrite(_cmdPin, (1 == i_cmdMode) ? HIGH : LOW);
  reset();

  //Serial1.begin((1 == i_cmdMode) ? 38400 : 57600);
  Serial1.begin((1 == i_cmdMode) ? 38400 : 9600);
}

// Reset the BT module over the reset pin.
void HC_05::reset(void)  //200, 2000. 500
{
  PRINTLN("BtReset");    
  BTSERIAL.flush();
  delay(200);
  // Using the reset pin
  digitalWrite(_resetPin, LOW);
  delay(2000);
  digitalWrite(_resetPin, HIGH);
  delay(500);
}

void HC_05::cmdStart() {
  setCmdMode(1);
}

void HC_05::cmdEnd() {
  setCmdMode(0); 
}

void HC_05::sendCommand(String cmdStr) {
  //Serial1.begin(38400);
  cmdStart();
  BTSERIAL.println("AT");
  delay(1000);
  BTSERIAL.println(cmdStr);
  PRINTLN(cmdStr);
  delay(1000);
  cmdEnd(); 
}

