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
          nofSensors{nofSensors}
    {
        buffer = new String[nofSensors]{};
    }

    void in(Stream &stream)
    {
        commandType = stream.available() ? (CommandType)stream.peek() : commandType;

        if (commandType == CommandTypeAction)
        {
            ActionCommand *actionCommand = new ActionCommand{stream, actions};
            actionCommand->startAction();
        }

        if (commandType == CommandTypeStep)
        {
            cycle->setStep(stream, actions);
        }
        if (commandType == CommandTypeCycle)
        {
            cycle = new Cycle{stream, actions};
        }
        if (commandType == CommandTypeCycle || commandType == CommandTypeStep)
        {
            for (auto index = 0; index < NofSensors; index++)
            {
                buffer[index] = sensors[index]->get();
                cycle->tryRunNextAction(buffer[index].toInt(), index);
            }
        }
    }

    void out(Print &stream)
    {
        stream << '[';
        for (auto index = 0; index < NofSensors; index++)
        {
            String actualValue = commandType == CommandTypeCycle ? buffer[index] : sensors[index]->get();
            stream << actualValue << (index == NofSensors - 1 ? ']' : ',');
        }
    }

  private:
    CommandType commandType;
    Sensor **sensors;
    Action **actions;
    Cycle *cycle;
    uint8_t nofSensors;
    uint8_t nofActions;
    String *buffer;
};

#endif