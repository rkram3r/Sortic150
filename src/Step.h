#pragma once
#include <Arduino.h>
#include <Condition.h>

struct Step
{
    uint8_t nofConditions;
    Condition **conditions;

    Step(Stream &stream) : nofConditions{(uint8_t)stream.parseInt()}
    {
        conditions[nofConditions] = {};
        for (auto index = 0; index < nofConditions; index++)
        {
            conditions[index] = new Condition{stream};
        }
    }

    bool conditionFullified(uint8_t valueToCompare)
    {
        for (auto index = 0; index < nofConditions; index++)
        {
            if (conditions[index]->conditionFullified(valueToCompare))
            {
                return true;
            }
        }

        return false;
    }

    bool nextStep(uint8_t valueToCompare)
    {
        for (auto index = 0; index < nofConditions; index++)
        {
            if (conditions[index]->goToNextStep)
            {
                return true;
            }
        }

        return false;
    }
};
