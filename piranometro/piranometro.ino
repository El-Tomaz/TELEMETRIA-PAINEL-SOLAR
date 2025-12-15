//
//    FILE: HX_read_median.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: HX711 demo
//     URL: https://github.com/RobTillaart/HX711



#include "HX711.h"

HX711 scale;

//  adjust pins if needed
uint8_t dataPin = 26;
uint8_t clockPin = 25;

uint32_t start, stop;
volatile float f;


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("HX711_LIB_VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  scale.begin(dataPin, clockPin);

  //  TODO find a nice solution for this calibration..
  //  load cell factor 20 KG
//  scale.set_scale(127.15);       //  TODO you need to calibrate this yourself.

  //  load cell factor 5 KG
  //  scale.set_scale(420.0983);
  //  reset the scale to zero = 0
//  scale.tare();

  Serial.println("\nPERFORMANCE");
  start = micros();
  
}


void loop()
{
  //  continuous scale once per second
  f = scale.read() * 3.3 / (1 << 23) / 128 * 1000000 * 1.22;
  float wm = f / 17.47;

  Serial.print("Tensão (uV):");
  Serial.print(f);
  Serial.print("\t Irradiância global (W/m²):");
  Serial.println(wm);
  
  delay(1000);
}


//  -- END OF FILE --

