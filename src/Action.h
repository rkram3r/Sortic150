#ifndef Action_h
#define Action_h
#include <Arduino.h>

class Action
{
public:
  virtual void start(uint8_t params) = 0;
};

#endif
