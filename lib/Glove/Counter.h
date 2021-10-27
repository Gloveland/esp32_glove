#ifndef COUNTER_H
#define COUNTER_H

#include <Wire.h>

class Counter {
 private:
  int eventNumber_ = 0;
  float referenceTimestampMs_ = 0;
  float lastMeasurementTimestampMs_ = 0;

 public:
  Counter() = default;

  void resetCounter();

  float getAndUpdateElapsedTimeSinceLastMeasurementMs();

  int getAndUpdateCounter();
};

#endif
