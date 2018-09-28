#include <Action.h>
#include <Sensor.h>
#include <Arduino.h>

#ifndef StreamHandler_h
#define StreamHandler_h
#define bufferSize 16

class StreamHandler
{
  public:
    StreamHandler(Sensor **sensors, Action **actions, uint8_t nofSensors, uint8_t nofActions) : sensors{sensors},
                                                                                                actions{actions},
                                                                                                nofSensors{nofSensors},
                                                                                                nofActions{nofActions}
    {
        sensorActionMapper = new uint8_t[nofActions]{};
        buffer = new uint8_t[bufferSize]{};
    }

    void printToStream(Print &out)
    {
        out << '[';
        for (auto index = 0; index < nofSensors; index++)
        {
            out << sensors[index]->get();
            out << (index == nofSensors - 1 ? ']' : ',');
        }
    }

    bool startAction(Stream &stream)
    {
        if (stream.available() && stream.peek() == 'a')
        {
            actionIndex = stream.parseInt();
            uint8_t params = stream.parseInt();
            if (stream.peek() != ')')
            {
                uint8_t sensorIndex = stream.parseInt();
                sensorActionMapper[actionIndex] = sensorIndex;
                bufferIndex = 0;
            }
            actions[actionIndex]->start(params);
            return true;
        }
        return false;
    }

    void interpret(Print &out)
    {
        String sensorData = sensors[actionIndex]->get();
        if (!sensorData.indexOf("\""))
        {
            uint8_t data = sensorData.toInt();
            if (bufferIndex == bufferSize)
            {
                buffer[bufferIndex] = data;
            }
        }

        out << sensorData;
    }

  private:
    Sensor **sensors;
    Action **actions;
    uint8_t *sensorActionMapper;
    uint8_t bufferIndex;
    uint8_t nofSensors;
    uint8_t nofActions;
    uint8_t actionIndex;
    uint8_t *buffer;
};

#endif
