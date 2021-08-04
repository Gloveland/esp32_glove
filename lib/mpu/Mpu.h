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

enum mpuAccScale {
  _2 = 16384,
  _4 = 8192,
  _8 = 4096,
  _16 = 2048,
};

enum mpuGyroRange {
  _250_DEG = 0x00,   ///< +/- 250 deg/s (default value)
  _500_DEG = 0x08,   ///< +/- 500 deg/s
  _1000_DEG = 0x10,  ///< +/- 1000 deg/s
  _2000_DEG = 0x18,  ///< +/- 2000 deg/s
};

enum mpuGyroScaleX10 {
  _250 = 1310,  /// 131
  _500 = 655,   /// 65.5
  _1000 = 328,  /// 32.8
  _2000 = 164,  /// 16.4
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
  Mpu(const std::string name, const int ad0);
  void init(const mpuAccRange accRange, const mpuGyroRange gyroRange);
  void init();
  void calibrate();
  SensorMeasurement read();
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
  const float GYRO_SCALE_DIVISOR = 10.0;
  const int ALL_REGISTERS = 14;
  const int BITS_IN_BYTE = 8;
  const std::string name;
  const int ad0;
  mpuAccRange accRange;
  mpuGyroRange gyroRange;
  Acceleration accError;
  Gyro gyroError;
  Gyro previousGyro;
  Gyro deviation;
  Acceleration inclinationfromAccError;
  Gyro inclinationAngle;
  float previousTime;
  float getAccScale(const mpuAccRange accRange);
  float getGyroScale(const mpuGyroRange gyroRange);
  void beginCommunication();
  void endCommunication();
  void checkAddress(int address);
  RawMeasurement readAllRaw();
  Acceleration readAcc(const int16_t rawAccX, const int16_t rawAccY,
                       const int16_t rawAccZ, const bool debug);
  Gyro readGyro(const int16_t rawGyroX, const int16_t rawGyroY,
                const int16_t rawGyroZ, const bool debug);
  Inclination readInclination(const Acceleration acc, const Gyro gyro,
                              const float elapsedTime, const bool debug);
  float readTemperature(const int16_t rawTemp);
  float calculateAccAngleX(const Acceleration acc);
  float calculateAccAngleY(const Acceleration acc);
};

#endif  // MPU_H
