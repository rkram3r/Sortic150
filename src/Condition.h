#pragma once
#include <Arduino.h>

typedef bool (*compare)(int a, int b);

bool equal(int a, int b) { return a == b; }
bool lower(int a, int b) { return a < b; }
bool greater(int a, int b) { return a > b; }
bool notEqual(int a, int b) { return !equal(a, b); }
bool lowerThanEqual(int a, int b) { return equal(a, b) || lower(a, b); }
bool greaterThanEqual(int a, int b) { return equal(a, b) || greater(a, b); }

compare functions[] = {equal, lower, greater, notEqual, lowerThanEqual, greaterThanEqual};

struct Condition
{
    bool goToNextStep;
    uint8_t sensorIndex;
    uint8_t comperator;
    int value;
    Condition(Stream &stream) : goToNextStep{stream.peek() == '*'},
                                sensorIndex{(uint8_t)stream.parseInt()},
                                comperator{(uint8_t)stream.read()},
                                value{(int)stream.parseInt()}
    {
    }

    bool conditionFullified(int valueToCompare)
    {
        return functions[comperator](valueToCompare, value);
    }
};
