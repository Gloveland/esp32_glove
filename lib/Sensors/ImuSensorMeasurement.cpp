#include <ImuSensorMeasurement.h>

ImuSensorMeasurement::ImuSensorMeasurement(Finger::Value finger,
                                           Acceleration acceleration, Gyro gyro)
    : finger_(finger), acc_(acceleration), gyro_(gyro) {}

const int ImuSensorMeasurement::kMtu = 64;
const std::string ImuSensorMeasurement::kMeasurementPacketFormat =
    "%d,%.3d,%c,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f";

std::string ImuSensorMeasurement::toPackage(int events_count, int elapsedTime) {
  char measurementBuffer[this->kMtu] = {0};
  memset(measurementBuffer, 0, this->kMtu);
  char fingerInitialLetter = Finger::getInitialLetter(this->finger_);
  int size_written = sprintf(
      measurementBuffer, ImuSensorMeasurement::kMeasurementPacketFormat.c_str(),
      events_count, elapsedTime, fingerInitialLetter, this->acc_.getX(),
      this->acc_.getY(), this->acc_.getZ(), this->gyro_.getX(),
      this->gyro_.getY(), this->gyro_.getZ());

  if (size_written > this->kMtu) {
    log_e("Error size written %d is bigger than buffer size!! %d", size_written,
          this->kMtu);
  }
  std::string blePackage = measurementBuffer;
  return measurementBuffer;
}
ImuSensorMeasurement::~ImuSensorMeasurement() = default;