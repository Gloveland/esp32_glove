#ifndef IMU_SENSOR_MEASUREMENT_H_
#define IMU_SENSOR_MEASUREMENT_H_

#include "Acceleration.h"
#include "Gyro.h"
#include "Inclination.h"
#include "Finger.h"

/**
 * The measurement of an IMU sensor.
 */
class ImuSensorMeasurement {
 public:
  ImuSensorMeasurement(Finger::Value finger, Acceleration acceleration, Gyro gyro,
                       Inclination inclination);
  Acceleration getAcc() const;
  Gyro getGyro() const;
  Inclination getInclination() const;
  ~ImuSensorMeasurement();

 private:
  Finger::Value finger_;
  Acceleration acc_;
  Gyro gyro_;
  Inclination inclination_;
};

#endif  // IMU_SENSOR_MEASUREMENT_H_
