#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <Wire.h>
#include <float.h>
#include <map>
#include "Finger.h"

#include "../Sensors/Accelerometer.h"
#include "../Sensors/Acceleration.h"

#include "../Sensors/Gyroscope.h"
#include "../Sensors/Gyro.h"

#include "../Sensors/InclinationCalculator.h"
#include "../Sensors/Inclination.h"
#include "../Sensors/ImuSensorMeasurement.h"

enum mpuBand {
  _260_HZ = 0x00,
  _184_HZ = 0x01,
  _94_HZ = 0x02,
  _44_HZ = 0x03,
  _21_HZ = 0x04,
  _10_HZ = 0x05,
  _5_HZ = 0x06,
};


enum mpuAddress {
  _ON = 0x68,
  _OFF = 0x69,
};

struct RawMeasurement {
  int16_t accX;
  int16_t accY;
  int16_t accZ;
  int16_t temp;
  int16_t gyroX;
  int16_t gyroY;
  int16_t gyroZ;
};

class Mpu {
 public:
  Mpu(const Finger::Value& finger);
  void init();
  void setWriteMode();
  void calibrate();
  void log();
  ImuSensorMeasurement read();
  Finger::Value getFinger();
  float getGyroScale(const GyroRange gyroRange);
  ~Mpu();

 private:
  const int GRAVITY_EARTH = 9.80665F;
  const int GENERAL_CONFIG = 0x1A;  ///< General configuration register
  const int PWR_MGMT_1 = 0x6B;
  const int RESET = 0x00;
  const int ACC_CONFIG_REGISTER = 0x1C;
  const int GYRO_CONFIG_REGISTER = 0x1C;
  const int ACCEL_XOUT_H = 0x3B;
  const int GYRO_XOUT_H = 0x43;
  const int ALL_REGISTERS = 14;
  const int BITS_IN_BYTE = 8;

  bool debug;
  Finger::Value finger_;
  u_int ad0_{};
  Accelerometer accelerometer;
  Gyroscope gyroscope;
  InclinationCalculator inclination_calculator;
  float previousTime_;

  void beginCommunication();
  void endCommunication();
  void checkAddress(int address);
  RawMeasurement readAllRaw();
 
  float readTemperature(const int16_t rawTemp);

};

#endif  // MPU_H
