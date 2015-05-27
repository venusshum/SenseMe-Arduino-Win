/**
* \defgroup SenseMe LEDMatrix SenseMeLEDMatrix Driver
*  SenseMe LEDMtrax utilises Adafruit Backpack and GFX drivers
* 
* @{
*/

/**
* \file 
* 		LED matrix driver
* \author
* 		Venus Shum
*/


//define SenseMeLEDMatrix Matrix and LEDMatrix

#include "Adafruit_LEDBackpack.h"




//#ifndef __SENSEMELEDMATRIX_H__
//#define __SENSEMELEDMATRIX_H__

class SenseMeLEDMatrixClass : public Adafruit_8x8matrix {
 public:
  SenseMeLEDMatrixClass(void);
  void begin(void);
  void setFace(String mood);
  void scrollText(String msg);
 private:


};

extern SenseMeLEDMatrixClass SenseMeLEDMatrix;

#define Matrix SenseMeLEDMatrix 
#define LEDMatrix SenseMeLEDMatrix 

//#endif

