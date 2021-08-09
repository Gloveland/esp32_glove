#ifndef GLOVE_H
#define GLOVE_H

#include <Mpu.h>
#include <map>

class Glove {
 public:
  Glove();
  void init();
  std::map<Finger::Value, ImuSensorMeasurement> readSensors();
  std::string getDeviceId();
  ~Glove();

 private:
  const int kSclPin = 22;
  const int kSdaPin = 21;
  std::string chipId;

  Mpu sensors_[2] = {
      Mpu(Finger::Value::kPinky),
      Mpu(Finger::Value::kThumb)
  };
  void setUpSensors();
  void calibrateSensors();
};

#endif  // GLOVE_H
