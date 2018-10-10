#pragma once
#include "CommandType.h"
#include "Cycle.h"
#include "ActionCommand.h"
#include "Sensor.h"
#include "Action.h"
#include "LoopbackStream.h"
#include <Arduino.h>

class MachineApi
{
  public:
    MachineApi(Sensor **sensors, uint8_t nofSensors, Action **actions, uint8_t nofActions)
        : commandType{None},
          sensors{sensors},
          nofSensors{nofSensors},
          actions{actions},
          nofActions{nofActions},
          buffer{}
    {
    }

    void in(Stream &stream)
    {
        commandType = stream.available() ? (CommandType)stream.peek() : commandType;

        if (commandType == CommandTypeAction)
        {
            ActionCommand *actionCommand = new ActionCommand{stream};
            actionCommand->tryRunAction(actions);
        }

        if (commandType == CommandTypeCycle)
        {
            cycle = new Cycle{stream, nofActions};
            for (auto index = 0; index < NofSensors; index++)
            {
                sensors[index]->get(buffer);
                auto actionCommand = cycle->getActionCommand(buffer.parseInt());
                actionCommand->tryRunAction(actions);
            }
        }
    }

    void out(Print &out)
    {
        out << '[';
        for (auto index = 0; index < NofSensors; index++)
        {
            if (commandType == CommandTypeCycle)
            {
                out << buffer.read();
            }
            else
            {
                sensors[index]->get(out);
            }
            out << (index == NofSensors - 1 ? ']' : ',');
        }
    }

  private:
    CommandType commandType;
    Sensor **sensors;
    uint8_t nofSensors;
    Action **actions;
    uint8_t nofActions;
    Cycle *cycle;
    LoopbackStream buffer;
};
