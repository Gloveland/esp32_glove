#ifndef IMU_SENSOR_MEASUREMENT_H_
#define IMU_SENSOR_MEASUREMENT_H_

#include <Arduino_JSON.h>

#include "Acceleration.h"
#include "Gyro.h"
#include "Inclination.h"


/**
 * The measurement of an IMU sensor.
 */
class ImuSensorMeasurement {

  public:
    ImuSensorMeasurement(Acceleration acceleration, Gyro gyro, Inclination inclination);
    Acceleration getAcc() const;
    Gyro getGyro() const;
    Inclination getInclination() const;
    JSONVar toJson() const;
    ~ImuSensorMeasurement();

  private: 
    Acceleration acc;
    Gyro gyro;
    Inclination inclination;
    float temperature;
};

#endif // IMU_SENSOR_MEASUREMENT_H_
