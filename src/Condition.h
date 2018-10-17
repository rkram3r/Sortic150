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
    uint8_t nofConditions;
    uint8_t *sensorIndexes;
    uint8_t *comperators;
    int *values;

    Condition(String income, uint8_t nofConditions) : nofConditions{nofConditions},
                                                      sensorIndexes{new uint8_t[nofConditions]{}},
                                                      comperators{new uint8_t[nofConditions]{}},
                                                      values{new int[nofConditions]{}}
    {
        char buff[32];
        income.toCharArray(buff, 64);

        char delimiter[] = "&";
        char *ptr;
        for (int index = 0; index < nofConditions; index++)
        {
            ptr = strtok(index == 0 ? buff : NULL, delimiter);
            sensorIndexes[index] = ptr[0] - '0';
            comperators[index] = ptr[1] - 'a';
            String tmp = ptr;
            tmp = tmp.substring(2, tmp.length());
            values[index] = tmp.toInt();
        }
    }

    bool conditionFullified(int valueToCompare, int currentSensorIndex)
    {
        for (int index = 0; index < nofConditions; index++)
        {
            if (currentSensorIndex == sensorIndexes[index] &&
                functions[comperators[index]](valueToCompare, values[index]))
            {
                return true;
            }
        }
        return false;
    }
};
