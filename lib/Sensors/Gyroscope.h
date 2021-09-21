#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include <Arduino.h>

#include "Gyro.h"

enum GyroRange {
  _250_DEG = 0x00,   ///< +/- 250 deg/s (default value)
  _500_DEG = 0x08,   ///< +/- 500 deg/s
  _1000_DEG = 0x10,  ///< +/- 1000 deg/s
  _2000_DEG = 0x18,  ///< +/- 2000 deg/s
};

enum GyroScaleX10 {
  _250 = 1310,  /// 131
  _500 = 655,   /// 65.5
  _1000 = 328,  /// 32.8
  _2000 = 164,  /// 16.4
};

class Gyroscope {
 private:
  const float GYRO_SCALE_DIVISOR = 10.0;
  GyroRange gyro_range_;
  Gyro gyro_error_;
  Gyro previous_gyro_;
  Gyro deviation_;
  float getGyroScale(const GyroRange gyro_range);
  void logGyroError();
  void logDeviation();

 public:
  Gyroscope(const GyroRange gyroRange);
  void setGyroError(int times, float sumGyro_x, float sumGyro_y, float sumGyro_z);
  void setDeviation(int times, float maxX, float maxY, float maxZ, float minX,
                    float minY, float minZ);
  Gyro readGyro(const int16_t rawGyro_x, const int16_t rawGyro_y,
                const int16_t rawGyro_z);
  GyroRange getGyroRange();

  ~Gyroscope();
};

#endif  // GYROSCOPE_H_
