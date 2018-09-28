#include <Adafruit_MotorShield.h>
#include <Action.h>

#ifndef Motor_h
#define Motor_h

class Motor : public Action
{
public:
  Motor(Adafruit_DCMotor *motor, byte direction) : motor{motor}, direction{direction}
  {
  }

  void start(uint8_t params)
  {
    motor->run(direction);
    motor->setSpeed(params);
  }

private:
  Adafruit_DCMotor *motor;
  byte direction;
};

#endif