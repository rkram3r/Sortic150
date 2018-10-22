#pragma once

#include <Arduino.h>
#include "ActionCommand.h"
#include "Condition.h"

class Step
{
  public:
    Step() : actionIndex{}, params{}, nofConditions{} {}
    Step(Stream &stream) : nofConditions{stream.parseInt()}
    {
        for (auto index = 0; index < nofConditions; index++)
        {
            conditions[index] = new Condition{stream};
        }
        actionIndex = {stream.parseInt()};
        params = {stream.parseInt()};

        Serial << nofConditions << " " << actionIndex << " " << params << '\n';
    }

    ActionCommand *getActionCommand(int valueToCompare, int sensorIndex)
    {

        for (int index = 0; index < nofConditions; index++)
        {

            if (conditions[index]->conditionFullified(valueToCompare, sensorIndex))
            {
                Serial << nofConditions << " " << actionIndex << " " << params << " " << index << '\n';

                return new ActionCommand{actionIndex, params};
            }
        }
        return dummyActionCommand;
    }

  private:
    uint8_t actionIndex;
    uint8_t params;
    uint8_t nofConditions;
    Condition *conditions[8];
    ActionCommand *dummyActionCommand{new ActionCommand{}};
};