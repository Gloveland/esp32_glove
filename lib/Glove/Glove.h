#ifndef GLOVE_H
#define GLOVE_H

#include <GloveMeasurements.h>
#include <Mpu.h>
#include <esp_task.h>

#include <map>

class Glove {
 public:
  void init();
  typedef std::map<Finger::Value, Mpu> GloveSensors;
  GloveMeasurements readSensors();
  static std::string getDeviceId();
  ~Glove();

 private:
  const int kSclPin = 22;
  const int kSdaPin = 21;
  /** Enabled sensors of the glove. */
  static const GloveSensors sensors_;

  void setUpSensors();
  void calibrateSensors();
};

#endif  // GLOVE_H
