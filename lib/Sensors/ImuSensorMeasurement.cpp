#include <ImuSensorMeasurement.h>

ImuSensorMeasurement::ImuSensorMeasurement(Acceleration acceleration, Gyro gyro,
                                           Inclination inclination)
    : acc(acceleration), gyro(gyro), inclination(inclination) {}

Acceleration ImuSensorMeasurement::getAcc() const { return this->acc; }

Gyro ImuSensorMeasurement::getGyro() const { return this->gyro; }

Inclination ImuSensorMeasurement::getInclination() const {
  return this->inclination;
}
ImuSensorMeasurement::~ImuSensorMeasurement() = default;