#include <ImuSensorMeasurement.h>

ImuSensorMeasurement::ImuSensorMeasurement(Finger::Value finger,
                                           Acceleration acceleration, Gyro gyro,
                                           Inclination inclination,
                                           float temperature)
    : finger_(finger),
      acc_(acceleration),
      gyro_(gyro),
      inclination_(inclination),
      temperature_(temperature) {
}

Acceleration ImuSensorMeasurement::getAcc() const { return this->acc_; }

Gyro ImuSensorMeasurement::getGyro() const { return this->gyro_; }

Inclination ImuSensorMeasurement::getInclination() const {
  return this->inclination_;
}
ImuSensorMeasurement::~ImuSensorMeasurement() = default;