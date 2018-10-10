#pragma once

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
  virtual Print &get(Print &print) = 0;
};
