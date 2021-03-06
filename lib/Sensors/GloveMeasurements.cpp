#include <GloveMeasurements.h>
const int GloveMeasurements::kImuSensorsAmount = 5;
const int GloveMeasurements::kMtu = 512;

const std::string GloveMeasurements::kGloveMeasurementsPacketFormat =
    "%d\n%lu\nP%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nR%.3f,%.3f,%.3f,%"
    ".3f,%.3f,%.3f\nM%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nI%.3f,%.3f,"
    "%.3f,%.3f,%.3f,%.3f\nT%.3f,%.3f,%.3f,%.3f,%.3f,%.3f;";

GloveMeasurements::GloveMeasurements() = default;

ImuSensorMeasurement GloveMeasurements::getSensor(const Finger::Value finger) {
  std::map<const Finger::Value, ImuSensorMeasurement>::iterator it =
      this->imuSensorMeasurementMap_.find(finger);
  if (it == this->imuSensorMeasurementMap_.end()) {
    log_e("Error getting a finger measurement");
    ImuSensorMeasurement zeroMesurement(finger, Acceleration(0, 0, 0),
                                        Gyro(0, 0, 0));
    return zeroMesurement;
  }
  return it->second;
}

void GloveMeasurements::setSensorMeasurementsMap(
    const std::map<const Finger::Value, ImuSensorMeasurement>
        imuSensorMeasurementMap) {
  this->imuSensorMeasurementMap_ = imuSensorMeasurementMap;
}

std::string GloveMeasurements::toPackage(int eventsCount, int timestampMs) {
  char measurementBuffer[this->kMtu] = {0};
  memset(measurementBuffer, 0, this->kMtu);
  ImuSensorMeasurement pinky = (this->getSensor(Finger::kPinky));
  ImuSensorMeasurement ring = (this->getSensor(Finger::kRing));
  ImuSensorMeasurement middle = (this->getSensor(Finger::kMiddle));
  ImuSensorMeasurement index = (this->getSensor(Finger::kIndex));
  ImuSensorMeasurement thumb = (this->getSensor(Finger::kThumb));

  int size_written = sprintf(
      measurementBuffer,
      GloveMeasurements::kGloveMeasurementsPacketFormat.c_str(), eventsCount,
      timestampMs,

      pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(),
      pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(),

      ring.getAcc().getX(), ring.getAcc().getY(), ring.getAcc().getZ(),
      ring.getGyro().getX(), ring.getGyro().getY(), ring.getGyro().getZ(),

      middle.getAcc().getX(), middle.getAcc().getY(), middle.getAcc().getZ(),
      middle.getGyro().getX(), middle.getGyro().getY(), middle.getGyro().getZ(),

      index.getAcc().getX(), index.getAcc().getY(), index.getAcc().getZ(),
      index.getGyro().getX(), index.getGyro().getY(), index.getGyro().getZ(),

      thumb.getAcc().getX(), thumb.getAcc().getY(), thumb.getAcc().getZ(),
      thumb.getGyro().getX(), thumb.getGyro().getY(), thumb.getGyro().getZ());

  if (size_written > this->kMtu) {
    log_e("Error size written %d is bigger than buffer size!! %d", size_written,
          this->kMtu);
  }
  std::string blePackage = measurementBuffer;
  return measurementBuffer;
}

bool GloveMeasurements::isNotComplete() {
  return this->imuSensorMeasurementMap_.size() <
         GloveMeasurements::kImuSensorsAmount;
}

GloveMeasurements::~GloveMeasurements() = default;
