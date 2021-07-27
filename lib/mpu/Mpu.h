#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <Movement.h>
#include <Wire.h>
#include <float.h>


enum mpuBand {
  _260_HZ = 0x00,
  _184_HZ = 0x01,
  _94_HZ = 0x02,
  _44_HZ = 0x03,
  _21_HZ = 0x04,
  _10_HZ = 0x05,
  _5_HZ = 0x06,
};

enum mpuAccRange {
  _2_G = 0x00,   /// 00000   +/- 2g (default value)
  _4_G = 0x08,   /// 01000   +/- 4g
  _8_G = 0x10,   /// 10000   +/- 8g
  _16_G = 0x18,  /// 11000   +/- 16g
};

enum mpuGyroRange {
  _250_DEG = 0x00,   ///< +/- 250 deg/s (default value)
  _500_DEG = 0x08,   ///< +/- 500 deg/s
  _1000_DEG = 0x10,  ///< +/- 1000 deg/s
  _2000_DEG = 0x18,  ///< +/- 2000 deg/s
};

enum mpuAddress{
  _ON = 0x68,
  _OFF = 0x69,
};


class Mpu {
 public:
  Mpu(const std::string name, const int ad0);
  void init();
  void calibrate();
  SensorMeasurement read();
  ~Mpu();

 private:
  const int GRAVITY_EARTH = 9.80665F;
  const int GENERAL_CONFIG = 0x1A;  ///< General configuration register
  const int ACC_CONFIG_REGISTER = 0x1C;
  const int GYRO_CONFIG_REGISTER = 0x1C;
  const int ACCEL_XOUT_H = 0x3B;
  const int GYRO_XOUT_H = 0x43;
  const std::string name;
  const int ad0;
  float AccErrorX, AccErrorY, AccErrorZ;
  float AccAngleErrorX, AccAngleErrorY;
  float GyroErrorX, GyroErrorY, GyroErrorZ;
  float GyroX, GyroY, GyroZ;
  float PrevGyroX, PrevGyroY, PrevGyroZ;
  float DeviationX, DeviationY, DeviationZ;
  float DegreesDeviationX, DegreesDeviationY, DegreesDeviationZ;
  float PreviousTime, CurrentTime, ElapsedTime;
  float GyroAngleX, GyroAngleY, GyroAngleZ;
  void beginCommunication();
  void endCommunication();
  void checkAddress(int address);
  Acceleration readAcceleration();
  Gyro readGyro();
  Inclination readInclination(Acceleration acc);
  float readTemperature();
  float calculateAccAngleX(float accX, float accY, float accZ);
  float calculateAccAngleY(float accX, float accY, float accZ);
};

#endif  // MPU_H
