#ifndef GLOVE_H
#define GLOVE_H

#include <Mpu.h>
#include <esp_task.h>

#include <map>

#include "../Sensors/GloveMeasurements.h"

typedef std::map<Finger::Value, Mpu> GloveSensors;

class Glove {
 public:
  void init();
  GloveMeasurements readSensors();
  static std::string getDeviceId();
  void calibrateSensors();
  ~Glove();

 private:
  static const int kChipIdStrSize;
  static const std::string kChipIdFormat;
  static const int kChipIdSize;
  static const int kSclPin;
  static const int kSdaPin;
  static const int OK;
  static const int DATA_BUFFER_ERROR;
  static const int NACK_ERROR;
  static const int UNKNOWN_ERROR;
  static const int kI2cSpeedHertz;
  /** Enabled sensors of the glove. */
  static const GloveSensors sensors_;

  void setUpSensors();
  bool checkAddress(int address);
};

#endif  // GLOVE_H
