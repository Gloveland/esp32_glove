#include "Counter.h"

 void Counter::resetCounter() {
    this->eventNumber_ = 0;
    this->referenceTimestampMs_ = millis();
    this->lastMeasurementTimestampMs_ = 0;
  }

  float Counter::getAndUpdateElapsedTimeSinceLastMeasurementMs() {
    float elapsedTimeMs =
        millis() - referenceTimestampMs_ - lastMeasurementTimestampMs_;
    this->lastMeasurementTimestampMs_ += elapsedTimeMs;
    return elapsedTimeMs;
  }

  /** Increments and returns the new counter value. */
  int Counter::getAndUpdateCounter() {
    eventNumber_++;
    return eventNumber_;
  }