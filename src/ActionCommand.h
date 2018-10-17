#pragma once
#include <Arduino.h>
#include "Action.h"

struct ActionCommand
{
    int actionIndex;
    uint8_t params;
    ActionCommand() : actionIndex{-1} {}

    ActionCommand(Stream &stream) : actionIndex{(int)stream.parseInt()},
                                    params{(uint8_t)stream.parseInt()}
    {
    }
    ActionCommand(int actionIndex, uint8_t params) : actionIndex{actionIndex},
                                                     params{params}
    {
    }

    bool tryRunAction(Action **actions)
    {
        if (actionIndex != -1)
        {
            actions[actionIndex]->start(params);
            return true;
        }
        return false;
    }
};
