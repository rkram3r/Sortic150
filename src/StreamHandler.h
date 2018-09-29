
#ifndef StreamHandler_h
#define StreamHandler_h

struct ActionCommand
{
    uint8_t actionIndex;
    uint8_t params;
    ActionCommand(Stream &stream) : actionIndex{stream.parseInt()},
                                    params{stream.parseInt()}
    {
    }
};

struct ActionGradient
{
    uint8_t *gradients;
    uint8_t tmpNumber{};
    uint8_t nofCalculations{0};
    ActionGradient()
    {
        gradients = new uint8_t[3]{};
    }

    void record(uint8_t actualValue)
    {
        if (nofCalculations == 255)
        {
            return;
        }

        if (nofCalculations != 0)
        {
            gradients[0] += actualValue < tmpNumber ? 1 : 0;
            gradients[1] += actualValue == tmpNumber ? 1 : 0;
            gradients[2] += actualValue > tmpNumber ? 1 : 0;
        }

        tmpNumber = actualValue;
        nofCalculations++;
    }
};

struct Interpreter
{
    uint8_t nofSensors;
    uint8_t nofActions;
    ActionGradient **actionGradients;
    uint8_t *sensorGradients;
    uint8_t nofGradients{3};
    bool alreadyCalculateGradients{false};

    Interpreter(uint8_t nofSensors, uint8_t nofActions) : nofSensors{nofSensors}, nofActions{nofActions}
    {
        sensorGradients = new uint8_t[nofActions * nofGradients]{};
        for (auto index = 0; index < nofSensors; index++)
        {
            actionGradients[index] = new ActionGradient[nofActions]{};
        }
    }

    void record(uint8_t sensorIndex, uint8_t actionIndex, uint8_t value)
    {
        actionGradients[sensorIndex][actionIndex].record(value);
    }

    uint8_t getIndexOfBestGradient(uint8_t actionIndex, uint8_t gradientIndex)
    {
        uint8_t bestValue{};
        uint8_t indexOfBestValue{};
        for (auto sensorIndex = 0; sensorIndex < nofSensors; sensorIndex++)
        {
            uint8_t *gradients = actionGradients[sensorIndex][actionIndex].gradients;
            uint8_t gradient = gradients[gradientIndex];

            if (gradient > bestValue)
            {
                bestValue = gradient;
                indexOfBestValue = actionIndex;
            }
        }

        return indexOfBestValue;
    }

    uint8_t *getGradients()
    {
        if (!alreadyCalculateGradients)
        {
            for (auto actionIndex = 0; actionIndex < nofActions; actionIndex++)
            {
                for (auto gradientIndex = 0; gradientIndex < nofGradients; gradientIndex++)
                {
                    sensorGradients[actionIndex * nofGradients + gradientIndex] = getIndexOfBestGradient(actionIndex, gradientIndex);
                }
            }
            alreadyCalculateGradients = true;
        }
        return sensorGradients;
    }

    uint8_t getIndexOfAction(uint8_t gradientType, uint8_t sensorIndex)
    {
        for (auto index = 0; index < nofActions * nofGradients; index++)
        {
            uint8_t actionForSensor = getGradients()[index * nofGradients + gradientType];
            if (actionForSensor == sensorIndex)
            {
                return actionForSensor;
            }
        }

        return -1;
    }
};

struct CycleCommand
{
    uint8_t nofSteps;
    uint8_t actualStep{0};
    uint8_t *sensorIndexes;
    uint8_t *targetValues;
    CycleCommand(Stream &stream) : nofSteps{stream.parseInt()}
    {
        sensorIndexes = new uint8_t[nofSteps]{};
        targetValues = new uint8_t[nofSteps]{};
        for (auto index = 0; index < nofSteps; index++)
        {
            sensorIndexes[index] = stream.parseInt();
            targetValues[index] = stream.peek() == '?' ? '?' : stream.parseInt();
        }
    }

    uint8_t getIndexOfActualSensor()
    {
        return sensorIndexes[actualStep];
    }

    bool tryGoNextStep(uint8_t actualValue, Interpreter *interpreter)
    {
        if (actualStep == nofSteps)
        {
            actualStep = 0;
        }

        uint8_t targetValue = targetValues[actualStep];

        if (targetValue == '?')
        {
            return -1;
        }
        if (actualValue == targetValue)
        {
            actualStep++;
        }

        uint8_t gradientType = actualValue > targetValue ? 0 : (actualValue == targetValue ? 0 : 1);
        return interpreter->getIndexOfAction(gradientType, getIndexOfActualSensor());
    }
};

enum CommandType
{
    CommandTypeAction = 'a',
    CommandTypeCycle = 'c',
    None = 0
};

#endif
