#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>

template <class T>
inline Print &operator<<(Print &obj, T arg)
{
  obj.print(arg);
  return obj;
}

class Sensor
{
public:
  virtual String get() = 0;
};
#endif
