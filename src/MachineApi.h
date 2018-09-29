#include <StreamHandler.h>
#include <Sensor.h>
#include <Action.h>

#ifndef MachineApi_h
#define MachineApi_h

class MachineApi
{
  public:
    MachineApi(Sensor **sensors, uint8_t nofSensors, Action **actions, uint8_t nofActions)
        : commandType{None},
          sensors{sensors},
          actions{actions},
          nofActions{nofActions},
          nofSensors{nofSensors},
          indexOfAction{}
    {
        interpreter = new Interpreter{nofSensors, nofActions};
    }

    void in(Stream &stream)
    {
        commandType = stream.available() ? (CommandType)stream.peek() : commandType;

        if (commandType == CommandTypeAction)
        {
            auto actionCommand = new ActionCommand{stream};
            auto action = actions[actionCommand->actionIndex];
            action->start(actionCommand->params);
            indexOfAction = actionCommand->actionIndex;
        }

        if (commandType == CommandTypeCycle)
        {
            CycleCommand *cycleCommand = new CycleCommand{stream};
            String actualValue = sensors[cycleCommand->getIndexOfActualSensor()]->get();
            if (actualValue != "")
            {
                uint8_t actionIndex = cycleCommand->tryGoNextStep(actualValue.toInt(), interpreter);
                if (actionIndex != -1)
                {
                    actions[actionIndex]->start(100);
                }
            }
        }
    }

    void out(Print &stream)
    {
        stream << '[';
        for (auto index = 0; index < NofSensors; index++)
        {
            String actualValue = sensors[index]->get();
            if (commandType == CommandTypeAction && actualValue != "" && actualValue.indexOf("\"") == -1)
            {
                interpreter->record(index, indexOfAction, actualValue.toInt());
            }
            stream << actualValue << (index == NofSensors - 1 ? ']' : ',');
        }
    }

  private:
    CommandType commandType;
    Sensor **sensors;
    Action **actions;
    uint8_t nofSensors;
    uint8_t nofActions;
    Interpreter *interpreter;
    uint8_t indexOfAction;
};

#endif