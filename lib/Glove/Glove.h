#ifndef GLOVE_H
#define GLOVE_H

#include <Mpu.h>
#include <map>
#include <esp_task.h>
#include <GloveMeasurements.h>

class Glove {
 public:
  void init();
  GloveMeasurements readSensors();
  static std::string getDeviceId();
  ~Glove();

 private:
  const int kSclPin = 22;
  const int kSdaPin = 21;
  std::string chip_id_;

  /** Enabled sensors of the glove. */
  const std::map<Finger::Value, Mpu> sensors_
      {
        {Finger::Value::kPinky, Mpu(Finger::Value::kPinky)} //,
        //{Finger::Value::kThumb, Mpu(Finger::Value::kThumb)}
      };

  void setUpSensors();
  void calibrateSensors();
};

#endif  // GLOVE_H
