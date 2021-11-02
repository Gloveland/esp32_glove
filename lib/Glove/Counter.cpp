#include "Counter.h"

void Counter::resetCounter() {
  this->eventNumber_ = 0;
  this->lastMeasurementTimestampMs_ = millis();
}

float Counter::getAndUpdateElapsedTimeSinceLastMeasurementMs() {
  float currentTimeMs = millis();
  float elapsedTimeMs = currentTimeMs - lastMeasurementTimestampMs_;
  this->lastMeasurementTimestampMs_ = currentTimeMs;
  return elapsedTimeMs;
}

float Counter::getMeasurementMillis(){
  return this->lastMeasurementTimestampMs_;
}

/** Increments and returns the new counter value. */
int Counter::getAndUpdateCounter() {
  eventNumber_++;
  return eventNumber_;
}