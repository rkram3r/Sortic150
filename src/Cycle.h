#pragma once

#include <Arduino.h>
#include "ActionCommand.h"
#include "Step.h"

struct Cycle
{
    uint8_t nofSteps;
    uint8_t nofActionCommands;
    uint8_t actualStep{0};
    ActionCommand **actionCommands;
    Step **steps;
    ActionCommand *dummyActionCommand{};

    Cycle(Stream &stream, uint8_t nofActionCommands) : nofSteps{(uint8_t)stream.parseInt()},
                                                       nofActionCommands{nofActionCommands}
    {
        steps[nofSteps * nofActionCommands] = {};
        actionCommands[nofSteps * nofActionCommands] = {};
        for (auto index = 0; index < nofSteps * nofActionCommands; index++)
        {
            actionCommands[index] = new ActionCommand{stream, nofSteps % index};
            steps[index] = new Step{stream};
        }
    }

    ActionCommand *getActionCommand(int valueToCompare)
    {
        if (steps[actualStep]->conditionFullified(valueToCompare))
        {
            tryGoNextStep(valueToCompare);
            return actionCommands[actualStep];
        }
        return dummyActionCommand;
    }

    bool tryGoNextStep(int valueToCompare)
    {
        if (steps[actualStep]->nextStep(valueToCompare))
        {
            actualStep += actualStep == nofSteps ? 1 : -nofSteps;
            return true;
        }
        return false;
    }
};
