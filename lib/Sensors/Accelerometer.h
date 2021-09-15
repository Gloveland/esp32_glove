#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "Acceleration.h"
#include <Arduino.h>
#include <float.h>

enum Range {
  _2_G = 0x00,   /// 00000   +/- 2g (default value)
  _4_G = 0x08,   /// 01000   +/- 4g
  _8_G = 0x10,   /// 10000   +/- 8g
  _16_G = 0x18,  /// 11000   +/- 16g
};

enum Scale {
  _2 = 16384,
  _4 = 8192,
  _8 = 4096,
  _16 = 2048,
};


class Accelerometer {

 public:;
  Accelerometer(const Range range, const bool debug = false);
  Acceleration readAcc(const int16_t rawAccX, const int16_t rawAccY, const int16_t rawAccZ);
  Range getRange();
  void setError(int times, float sum_acc_x, float sum_acc_y, float sum_acc_z);
  ~Accelerometer();

 private:
  const int GRAVITY_EARTH = 9.80665F;
  Range range;
  bool debug;
  Acceleration acc_error;
 
  float getScale(const Range range);
  void logError();
};

#endif  // MPU_H
