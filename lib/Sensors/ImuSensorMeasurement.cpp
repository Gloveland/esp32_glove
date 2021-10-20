#include <ImuSensorMeasurement.h>

ImuSensorMeasurement::ImuSensorMeasurement(Finger::Value finger,
                                           Acceleration acceleration, Gyro gyro)
    : finger_(finger), acc_(acceleration), gyro_(gyro) {}

const std::string ImuSensorMeasurement::kMeasurementPacketFormat =
    "%d\n%.3f\n%c\n%.3f,%.3f,%.3f,%.3f,%.3f,%.3f;";

void ImuSensorMeasurement::toPackage(int events_count, int elapsedTime,
                                     char measurement_buffer_[],
                                     const int size) {
  memset(measurement_buffer_, 0, size);
  int size_written = sprintf(
      measurement_buffer_,
      ImuSensorMeasurement::kMeasurementPacketFormat.c_str(), events_count,
      elapsedTime, Finger::getInitialLetter(this->finger_), this->acc_.getX(),
      this->acc_.getY(), this->acc_.getZ(), this->gyro_.getX(),
      this->gyro_.getY(), this->gyro_.getZ());

  if (size_written > size) {
    log_e("Error size written %d is bigger than buffer size!! %d", size_written,
          size);
  }
}
ImuSensorMeasurement::~ImuSensorMeasurement() = default;