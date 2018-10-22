#pragma once
#include "CommandType.h"
#include "ActionCommand.h"
#include "Sensor.h"
#include "Action.h"
#include "Step.h"
#include <Arduino.h>
#include <LoopbackStream.h>

#define DEBUGBUFFER 256
LoopbackStream buffer(DEBUGBUFFER);

class MachineApi
{
  public:
    MachineApi(Sensor **sensors, uint8_t nofSensors, Action **actions, uint8_t nofActions)
        : commandType{None},
          sensors{sensors},
          nofSensors{nofSensors},
          actions{actions},
          nofActions{nofActions},
          steps{},
          nofSteps{}
    {
    }

    // s[2(1b0&1c38)1(100)]
    //c3[0b0&0c38:1(100),0b0&0d38:1(100),0b0&0a38:1(100)]
    void in(Stream &stream)
    {
        if (!stream.available())
        {
            return;
        }
        commandType = (CommandType)stream.peek();
        buffer << stream.readStringUntil(']');
        if (commandType == CommandTypeAction)
        {
            ActionCommand *actionCommand = new ActionCommand{buffer};
            actionCommand->tryRunAction(actions);
        }
        if (commandType == CommandTypeStep)
        {
            steps[nofSteps] = new Step{buffer};
            nofSteps++;
        }
    }

    void out(Stream &out)
    {
        if (commandType == CommandTypeStep)
        {
            out << '[';
            for (auto index = 0; index < NofSensors; index++)
            {
                sensors[index]->get(buffer);
                if (buffer.peek() != -1)
                {
                    auto value = buffer.parseInt();
                    out << value;
                    auto actionCommand = steps[actualStep]->getActionCommand(value, index);
                    actionCommand->tryRunAction(actions);
                }
                out << (index == NofSensors - 1 ? ']' : ',');
            }
        }
        else
        {
            out << '[';
            for (auto index = 0; index < NofSensors; index++)
            {
                sensors[index]->get(out) << (index == NofSensors - 1 ? ']' : ',');
            }
        }
    }

  private:
    CommandType commandType;
    Sensor **sensors;
    uint8_t nofSensors;
    Action **actions;
    uint8_t nofActions;
    Step *steps[16];
    uint8_t nofSteps{0};
    uint8_t actualStep{0};
};
