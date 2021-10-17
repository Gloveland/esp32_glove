#ifndef COUNTER_H
#define COUNTER_H

class Counter {
  private:
    int eventNumber_ = 0;
    float referenceTimestampMs_ = 0;
    float lastMeasurementTimestampMs_ = 0;
  public:
    
    Counter() = default;
    
    void resetCounter() {
      this->eventNumber_ = 0;
      this->referenceTimestampMs_ = millis();
    }

    float getElapsedTimeSinceReferenceMs() {
      return millis() - referenceTimestampMs_;
    }

    float getAndUpdateElapsedTimeSinceLastMeasurementMs() {
      float elapsedTimeMs = millis() - lastMeasurementTimestampMs_;
      this->lastMeasurementTimestampMs_ = millis();
      return elapsedTimeMs;
    }

    /** Increments and returns the new counter value. */
    int getAndUpdateCounter() {
      eventNumber_++;
      return eventNumber_;
    }

};

#endif
