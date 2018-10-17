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

    void out(Print &out)
    {
        if (commandType == CommandTypeCycle)
        {
            out << '[';
            for (auto index = 0; index < NofSensors; index++)
            {
                uint8_t *value = sensors[index]->get();
                if (value[0] != 0)
                {
                    out << value[1];
                    ActionCommand *actionCommand = cycle->getActionCommand(value[1], index);
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
                uint8_t *value = sensors[index]->get();
                if (value[0] != 0)
                {
                    out << value[1];
                }
                out << (index == NofSensors - 1 ? ']' : ',');
            }
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
