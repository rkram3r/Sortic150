#pragma once
#include "CommandType.h"
#include "Cycle.h"
#include "ActionCommand.h"
#include "Sensor.h"
#include "Action.h"
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
          nofActions{nofActions}
    {
    }
    //c3[0b0&0c38:1(100),0b0&0d38:1(100),0b0&0a38:1(100)]
    void in(Stream &stream)
    {
        if (!stream.available())
        {
            return;
        }
        String income = stream.readStringUntil('\n');
        commandType = (CommandType)income.charAt(0);
        if (commandType == CommandTypeAction)
        {
            ActionCommand *actionCommand = new ActionCommand{buffer};
            actionCommand->tryRunAction(actions);
        }
        if (commandType == CommandTypeCycle)
        {
            cycle = new Cycle{income, nofActions};
        }
    }

    void out(Stream &out)
    {
        if (commandType == CommandTypeCycle)
        {
            out << '[';
            for (auto index = 0; index < NofSensors; index++)
            {
                sensors[index]->get(buffer);
                if (buffer.peek() != -1)
                {
                    int value{buffer.parseInt()};
                    out << value;
                    ActionCommand *actionCommand = cycle->getActionCommand(value, index);
                    actionCommand->tryRunAction(actions);
                }
                out << (index == NofSensors - 1 ? ']' : ',');
            }
        }
        /* else //c3[0b0&0c38:1(100),0b0&0d38:1(100),0b0&0a38:1(100)]
        {
            out << '[';
            for (auto index = 0; index < NofSensors; index++)
            {
                sensors[index]->get(out) << (index == NofSensors - 1 ? ']' : ',');
            }
        }*/
    }

  private:
    CommandType commandType;
    Sensor **sensors;
    uint8_t nofSensors;
    Action **actions;
    uint8_t nofActions;
    Cycle *cycle{};
};
