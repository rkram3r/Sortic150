#ifndef Sleep_h
#define Sleep_h

#include <Action.h>
#include <Arduino.h>

class Sleep :public Action
{
public:
  void start(int time)
  {
    delay(time);
  }
};

#endif
