#pragma once

#include <NewPing.h>
#include <Sensor.h>

class Distance : public Sensor
{
public:
  Distance(NewPing *sensor) : sensor{sensor} {}

  Print &get(Print &out)
  {
    auto value = sensor->ping_cm();
    if (value == 0)
    {
      return out;
    }
    return out << value;
  }

private:
  NewPing *sensor;
};