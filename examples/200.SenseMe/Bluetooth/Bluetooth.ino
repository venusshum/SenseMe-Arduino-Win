#include <Arduino.h>
#include <SenseMeBluetooth.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SenseMeBluetooth.begin();
  delay(5000);
  SenseMeBluetooth.printVersion();
  delay(2000);
  //set a random device name
  SenseMeBluetooth.setModuleName("SenseMe" + String(random(1,9)));
  delay(2000);
  //get the Module name
  SenseMeBluetooth.getModuleName();
  //SenseMeBluetooth.getDeviceAddr();
  //SenseMeBluetooth.setSlave();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  char temChar;
  temChar = SenseMeBluetooth.readChar();
  if (temChar!=0) {
    Serial.print(temChar);
  }
}
