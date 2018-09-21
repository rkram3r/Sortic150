#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>

class Sensor
{
public:

  virtual Print & get(Print &obj) = 0;
};
#endif
