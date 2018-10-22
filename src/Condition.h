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

class Condition
{
public:
  Condition(Stream &stream) : sensorIndex{stream.parseInt()},
                              comperatorIndex{stream.peek() - 'a'},
                              value{stream.parseInt()}
  {
    Serial << sensorIndex << " " << comperatorIndex << " " << value << '\n';
  }

  bool conditionFullified(int valueToCompare, int currentSensorIndex)
  {
    return currentSensorIndex == sensorIndex &&
           functions[comperatorIndex](valueToCompare, value);
  }

private:
  uint8_t sensorIndex;
  uint8_t comperatorIndex;
  int value;
};
