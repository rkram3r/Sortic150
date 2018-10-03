
#ifndef StreamHandler_h
#define StreamHandler_h

struct ActionCommand
{
    uint8_t actionIndex;
    uint8_t params;
    Action **actions;
    ActionCommand(Stream &stream, Action **actions) : actionIndex{stream.parseInt()},
                                                      params{stream.parseInt()}
    {
    }
    void startAction()
    {
        actions[actionIndex]->start(params);
    }
};

struct Comparison
{
    uint8_t sensorIndex;
    uint8_t comperator;
    uint8_t value;
    bool unknown;

    Comparison(Stream &stream) : sensorIndex{stream.parseInt()},
                                 comperator{stream.read()},
                                 value{stream.peek() == 'X' ? 0 : stream.parseInt()},
                                 unknown{stream.peek() == 'X'}
    {
    }

    bool conditionFullifilled(uint8_t sensorValue, uint8_t index)
    {
        if (sensorIndex != index || unknown)
        {
            return false;
        }
        if (comperator == '<')
        {
            return value < sensorValue;
        }
        if (comperator == '>')
        {
            return value > sensorValue;
        }
        if (comperator == '!')
        {
            return value != sensorValue;
        }
        return value == sensorValue;
    }
};

struct Condition
{
    uint8_t nofComparisions;
    Comparison **comparisons;

    Condition(Stream &stream) : nofComparisions{stream.parseInt()}
    {
        comparisons[nofComparisions] = {};
        for (auto index = 0; index < nofComparisions; index++)
        {
            comparisons[index] = new Comparison{stream};
        }
    }

    bool conditionFullifilled(uint8_t sensorValue, uint8_t index)
    {
        for (auto index = 0; index < nofComparisions; index++)
        {
            if (!comparisons[index]->conditionFullifilled(sensorValue, index))
            {
                return false;
            }
        }
        return true;
    }
};

struct Step
{
    uint8_t nofConditions;
    Condition **conditions;
    ActionCommand **actionCommands;
    uint8_t *comperators;

    Step(Stream &stream, Action **actions) : nofConditions{stream.parseInt()}
    {
        conditions[nofConditions] = {};
        actionCommands[nofConditions] = {};
        comperators[nofConditions] = new uint8_t[nofConditions]{};

        for (auto index = 0; index < nofConditions; index++)
        {
            conditions[index] = new Condition{stream};
            actionCommands[index] = new ActionCommand{stream, actions};
        }
    }
    uint8_t indexOfFulifilledCondition(uint8_t sensorValue, uint8_t sensorIndex)
    {
        for (auto index = 0; index < nofConditions; index++)
        {
            Condition *condition = conditions[index];
            if (condition->conditionFullifilled(sensorValue, sensorIndex))
            {
                return index;
            }
        }

        return -1;
    }
};

struct Cycle
{
    uint8_t nofSteps;
    Step **steps;
    uint8_t actualStep{0};

    Cycle(Stream &stream, Action **actions) : nofSteps{stream.parseInt()}
    {
        steps[nofSteps] = {};
        for (auto index = 0; index < nofSteps; index++)
        {
            steps[index] = new Step{stream, actions};
        }
    }

    void tryRunNextAction(uint8_t sensorValue, uint8_t sensorIndex)
    {
        Step *step = steps[actualStep];
        uint8_t indexOfFulifilledCondition = step->indexOfFulifilledCondition(sensorValue, sensorIndex);

        if (indexOfFulifilledCondition != -1)
        {
            actualStep += actualStep == nofSteps ? -nofSteps : 1;
            ActionCommand *actionCommand = step->actionCommands[indexOfFulifilledCondition];
            actionCommand->startAction();
        }
    }

    void setStep(Stream &stream, Action **actions)
    {
        delete steps[actualStep];
        steps[actualStep] = new Step{stream, actions};
    }
};

enum CommandType
{
    CommandTypeAction = 'a',
    CommandTypeCycle = 'c',
    CommandTypeStep = 's',
    None = 0
};

#endif
