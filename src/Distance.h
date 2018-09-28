
#ifndef Distance_h
#define Distance_h

#include <NewPing.h>
#include <Sensor.h>

class Distance : public Sensor
{
public:
  Distance(NewPing *sensor) : sensor{sensor} {}

  String get()
  {
    auto value = sensor->ping_cm();
    if (value == 0)
    {
      return "";
    }
    return String(value);
  }

private:
  NewPing *sensor;
};

#endif