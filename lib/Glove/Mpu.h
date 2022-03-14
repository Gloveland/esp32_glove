#ifndef MPU_H
#define MPU_H

#include <Adafruit_MPU6050.h>
#include <Arduino.h>
#include <float.h>

#include <map>

#include "../Sensors/Acceleration.h"
#include "../Sensors/Accelerometer.h"
#include "../Sensors/Gyro.h"
#include "../Sensors/Gyroscope.h"
#include "../Sensors/ImuSensorMeasurement.h"
#include "Finger.h"

enum mpuAddress {
  _ON = 0x68,
  _OFF = 0x69,
};

class Mpu {
 public:
  Mpu(const Finger::Value& finger);
  void init();
  void sleep();
  void restart();
  void setWriteMode();
  void calibrate();
  void log();
  ImuSensorMeasurement read();
  Finger::Value getFinger();
  float getGyroScale(const GyroRange gyroRange);
  ~Mpu();

 private:
  static const int GRAVITY_EARTH;
  Adafruit_MPU6050 mpu_;
  Finger::Value finger_;
  u_int ad0_;
  Accelerometer accelerometer;
  Gyroscope gyroscope;
  float previousTime_;

  void beginCommunication();
  void endCommunication();
};

#endif  // MPU_H
