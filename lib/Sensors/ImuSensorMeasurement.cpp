#include <ImuSensorMeasurement.h>

ImuSensorMeasurement::ImuSensorMeasurement(Finger::Value finger,
                                           Acceleration acceleration, Gyro gyro)
    : finger_(finger),
      acc_(acceleration),
      gyro_(gyro){}

Acceleration ImuSensorMeasurement::getAcc() const { return this->acc_; }

Gyro ImuSensorMeasurement::getGyro() const { return this->gyro_; }

ImuSensorMeasurement::~ImuSensorMeasurement() = default;