
#ifndef Distance_h
#define Distance_h

#include <NewPing.h>
#include <Sensor.h>

class Distance:public Sensor
{
public:
  Distance(NewPing *sensor) : sensor{sensor} { }
    
  Print & get(Print &obj){
      return obj << sensor->ping_cm();
  }
private:
  NewPing *sensor;
};

#endif