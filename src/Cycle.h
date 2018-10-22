#pragma once

#include <Arduino.h>
#include "ActionCommand.h"
#include "Condition.h"

class Cycle
{
  public:
    Cycle(String income, uint8_t nofActionCommands) : nofSteps{(uint8_t)income.charAt(1) - '0'}

    {
        char buff[64];
        income.toCharArray(buff, 64);
        char delimiter[] = "[(),:]";
        char *ptr = strtok(buff, delimiter);
        String *conditionAsString = new String[nofSteps]{};
        nofActionCommands = {nofActionCommands};
        actionIndexes = new uint8_t[nofSteps]{};
        params = new uint8_t[nofSteps]{};

        for (int index = 0; index < nofSteps; index++)
        {
            ptr = strtok(NULL, delimiter);
            conditionAsString[index] = ptr;

            ptr = strtok(NULL, delimiter);
            actionIndexes[index] = atoi(ptr);

            ptr = strtok(NULL, delimiter);
            params[index] = atoi(ptr);
        }

        for (int index = 0; index < nofSteps; index++)
        {
            conditions[index] = new Condition{conditionAsString[index], 2};
        }

        Serial << nofSteps << '\n';
    }
    //c3[0b0&0c38:1(100),0b0&0d38:1(100),0b0&0a38:1(100)]
    ActionCommand *getActionCommand(int valueToCompare, int sensorIndex)
    {
        for (int index = 0; index < nofSteps; index++)
        {
            if (conditions[index]->conditionFullified(valueToCompare, sensorIndex))
            {
                Serial << "nice.";
                //  return new ActionCommand{actionIndexes[index], params[index]};
            }
        }
        return dummyActionCommand;
    }

  private:
    uint8_t nofSteps{};
    uint8_t nofActionCommands;
    uint8_t actualStep{0};
    uint8_t *actionIndexes;
    uint8_t *params;
    Condition *conditions[8];
    ActionCommand *dummyActionCommand{new ActionCommand{}};
};
