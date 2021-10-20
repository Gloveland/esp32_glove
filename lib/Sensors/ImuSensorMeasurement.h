#ifndef IMU_SENSOR_MEASUREMENT_H_
#define IMU_SENSOR_MEASUREMENT_H_

#include "Acceleration.h"
#include "Finger.h"
#include "Gyro.h"

/**
 * The measurement of an IMU sensor.
 */
class ImuSensorMeasurement {
 public:
  ImuSensorMeasurement(Finger::Value finger, Acceleration acceleration,
                       Gyro gyro);
  Acceleration getAcc() const;
  Gyro getGyro() const;
  void toPackage(int events_count, int elapsedTime, char *measurement_buffer_,
                 const int size);
  ~ImuSensorMeasurement();

 private:
  const static std::string kMeasurementPacketFormat;
  Finger::Value finger_;
  Acceleration acc_;
  Gyro gyro_;
};

#endif  // IMU_SENSOR_MEASUREMENT_H_
