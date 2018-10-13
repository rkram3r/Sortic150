#pragma once
#include <Arduino.h>

typedef bool (*compare)(int a, int b);

bool equal(int a, int b) { return a == b; }
bool lower(int a, int b) { return a < b; }
bool greater(int a, int b) { return a > b; }
bool notEqual(int a, int b) { return !equal(a, b); }
bool lowerThanEqual(int a, int b) { return equal(a, b) || lower(a, b); }
bool greaterThanEqual(int a, int b) { return equal(a, b) || greater(a, b); }
bool alwaysTrue(int a, int b) { return true; }
const uint8_t asciiValueForA = 97;

compare functions[] = {equal, lower, greater, notEqual, lowerThanEqual, greaterThanEqual, alwaysTrue};

struct Condition
{
    uint8_t sensorIndex;
    bool goToNextStep;
    char comperator;
    int value;
    Condition(Stream &stream) : sensorIndex{(uint8_t)stream.parseInt()},
                                goToNextStep{stream.peek() == '*'},
                                comperator{(char)stream.read()},
                                value{(int)stream.parseInt()} {}

    bool conditionFullified(int valueToCompare, uint8_t currentSensorIndex)
    {
        return currentSensorIndex == sensorIndex && functions[comperator - asciiValueForA](valueToCompare, value);
    }
};
