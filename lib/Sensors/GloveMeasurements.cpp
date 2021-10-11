#include <GloveMeasurements.h>
const int GloveMeasurements::kImuSensorsAmount = 5;

const std::string GloveMeasurements::kGloveMeasurementsPacketFormat =
    "%d-%.3f\nP%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nR%.3f,%.3f,%.3f,%.3f,%."
    "3f,%.3f,%.3f,%.3f,%.3f\nM%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nI%."
    "3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nT%.3f,%.3f,%.3f,%.3f,%.3f,%."
    "3f,%.3f,%.3f,%.3f;";

GloveMeasurements::GloveMeasurements() = default;

ImuSensorMeasurement GloveMeasurements::getSensor(const Finger::Value finger) {
  std::map<const Finger::Value, ImuSensorMeasurement>::iterator it =
      this->imuSensorMeasurementMap_.find(finger);
  if (it == this->imuSensorMeasurementMap_.end()) {
    log_e("Error getting a finger measurement");
    ImuSensorMeasurement zeroMesurement(finger, Acceleration(0, 0, 0),
                                        Gyro(0, 0, 0), Inclination(0, 0, 0));
    return zeroMesurement;
  }
  return it->second;
}

void GloveMeasurements::setSensorMeasurementsMap(
    const float elapsedTime,
    const std::map<const Finger::Value, ImuSensorMeasurement>
        imuSensorMeasurementMap) {
  this->elapsedTime_ = elapsedTime;
  this->imuSensorMeasurementMap_ = imuSensorMeasurementMap;
}

void GloveMeasurements::toPackage(int events_count,
                                  char glove_measurement_buffer_[],
                                  const int size) {
  memset(glove_measurement_buffer_, 0, size);
  ImuSensorMeasurement pinky = (this->getSensor(Finger::kPinky));
  ImuSensorMeasurement ring = (this->getSensor(Finger::kRing));
  ImuSensorMeasurement middle = (this->getSensor(Finger::kMiddle));
  ImuSensorMeasurement index = (this->getSensor(Finger::kIndex));
  ImuSensorMeasurement thumb = (this->getSensor(Finger::kThumb));

  int size_written = sprintf(
      glove_measurement_buffer_,
      GloveMeasurements::kGloveMeasurementsPacketFormat.c_str(), events_count, this->elapsedTime_,

      pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(),
      pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(),
      pinky.getInclination().getRoll(), pinky.getInclination().getPitch(),
      pinky.getInclination().getYaw(),

      ring.getAcc().getX(), ring.getAcc().getY(), ring.getAcc().getZ(),
      ring.getGyro().getX(), ring.getGyro().getY(), ring.getGyro().getZ(),
      ring.getInclination().getRoll(), ring.getInclination().getPitch(),
      ring.getInclination().getYaw(),

      middle.getAcc().getX(), middle.getAcc().getY(), middle.getAcc().getZ(),
      middle.getGyro().getX(), middle.getGyro().getY(), middle.getGyro().getZ(),
      middle.getInclination().getRoll(), middle.getInclination().getPitch(),
      middle.getInclination().getYaw(),

      index.getAcc().getX(), index.getAcc().getY(), index.getAcc().getZ(),
      index.getGyro().getX(), index.getGyro().getY(), index.getGyro().getZ(),
      index.getInclination().getRoll(), index.getInclination().getPitch(),
      index.getInclination().getYaw(),

      thumb.getAcc().getX(), thumb.getAcc().getY(), thumb.getAcc().getZ(),
      thumb.getGyro().getX(), thumb.getGyro().getY(), thumb.getGyro().getZ(),
      thumb.getInclination().getRoll(), thumb.getInclination().getPitch(),
      thumb.getInclination().getYaw()

  );

  if (size_written > size) {
    log_e("Error size is bigger than 512!!, size_written: %d", size_written);
  }
}

bool GloveMeasurements::isNotComplete() {
  return this->imuSensorMeasurementMap_.size() <
         GloveMeasurements::kImuSensorsAmount;
}

GloveMeasurements::~GloveMeasurements() = default;
