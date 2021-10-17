#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <Wire.h>
#include <float.h>

#include <map>

#include "../Sensors/Acceleration.h"
#include "../Sensors/Accelerometer.h"
#include "../Sensors/Gyro.h"
#include "../Sensors/Gyroscope.h"
#include "../Sensors/ImuSensorMeasurement.h"
#include "../Sensors/Inclination.h"
#include "../Sensors/InclinationCalculator.h"
#include "Finger.h"

enum mpuBand {
  _260_HZ = 0x00,
  _184_HZ = 0x01,
  _94_HZ = 0x02,
  _44_HZ = 0x03,
  _21_HZ = 0x04,
  _10_HZ = 0x05,
  _5_HZ = 0x06,
};

enum mpuI2cClock {
  _348_HZ = 0,
  _333_HZ = 1,
  _320_HZ = 2,
  _308_HZ = 3,
  _296_HZ = 4,
  _286_HZ = 5,
  _276_HZ = 6,
  _267_HZ = 7,
  _258_HZ = 8,
  _500_HZ = 9,
  _471_HZ = 10,
  _444_HZ = 11,
  _421_HZ = 12,
  _400_HZ = 13,
  _381_HZ = 14,
  _364_HZ = 15
};

enum mpuAddress {
  _ON = 0x68,
  _OFF = 0x69,
};

struct RawMeasurement {
  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;
  int16_t temp;
  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
};

class Mpu {
 public:
  Mpu(const Finger::Value& finger);
  void init();

  void setWriteMode();
  void setMasterClockSpeed();
  void calibrate();
  void log();
  ImuSensorMeasurement read();
  Finger::Value getFinger();
  float getGyroScale(const GyroRange gyroRange);
  ~Mpu();

 private:
  static const int HEX_ADDRESS;
  static const int OK;
  static const int GRAVITY_EARTH;
  static const int GENERAL_CONFIG;  ///< General configuration register
  static const int PWR_MGMT_1;
  static const int RESET;
  static const int ACC_CONFIG_REGISTER;
  static const int GYRO_CONFIG_REGISTER;
  static const int ACCEL_XOUT_H;
  static const int GYRO_XOUT_H;
  static const int ALL_REGISTERS;
  static const int BITS_IN_BYTE;
  static const int TEMP_DIS_PLL;
  static const int I2C_MST_CTRL;
  static const uint8_t I2C_CLK_MASK;

  Finger::Value finger_;
  u_int ad0_;
  Accelerometer accelerometer;
  Gyroscope gyroscope;
  InclinationCalculator inclination_calculator;
  float previousTime_;

  void beginCommunication();
  void endCommunication();
  void writeBits(const int registerAddress, uint8_t mask, uint8_t newVal);
  RawMeasurement readAllRaw();
};

#endif  // MPU_H
