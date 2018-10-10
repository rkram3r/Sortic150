#pragma once
#include <Arduino.h>

class Action
{
public:
  virtual void start(uint8_t params) = 0;
};
