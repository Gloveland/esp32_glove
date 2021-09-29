#include <GloveMeasurements.h>
const int GloveMeasurements::kImuSensorsAmount = 5;

const std::string GloveMeasurements::kglove_mesurements_packet_format =
    "%d\nP%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nR%.3f,%.3f,%.3f,%.3f,%."
    "3f,%.3f,%.3f,%.3f,%.3f\nM%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nI%."
    "3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nT%.3f,%.3f,%.3f,%.3f,%.3f,%."
    "3f,%.3f,%.3f,%.3f;";

GloveMeasurements::GloveMeasurements() = default;

ImuSensorMeasurement GloveMeasurements::getSensor(const Finger::Value finger) {
  std::map<const Finger::Value, ImuSensorMeasurement>::iterator it =
      this->imuSensorMeasurementMap_.find(finger);
  if (it == this->imuSensorMeasurementMap_.end()) {
    Serial.println("[ERROR] Error getting a finger measurement");
    ImuSensorMeasurement zeroMesurement(finger, Acceleration(0, 0, 0),
                                        Gyro(0, 0, 0), Inclination(0, 0, 0), 0);
    return zeroMesurement;
  }
  return it->second;
}

void GloveMeasurements::setSensorMeasurementsMap(
    const std::map<const Finger::Value, ImuSensorMeasurement>
        imuSensorMeasurementMap) {
  this->imuSensorMeasurementMap_ = imuSensorMeasurementMap;
}

void GloveMeasurements::toPackage(int events_count,
                                  char glove_measurement_buffer_[],
                                  const int size) {
  memset(glove_measurement_buffer_, 0, size);
  ImuSensorMeasurement pinky = (this->getSensor(Finger::kPinky));
  // ImuSensorMeasurement ring = *(this->ring_imu_measurement_);
  // ImuSensorMeasurement middle = *(this->middle_imu_measurement_);
  // ImuSensorMeasurement index = *(this->index_imu_measurement_);
  // ImuSensorMeasurement thump = *(this->thumb_imu_measurement_);

  int size_written = sprintf(
      glove_measurement_buffer_,
      GloveMeasurements::kglove_mesurements_packet_format.c_str(), events_count,
      pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(),
      pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(),
      pinky.getInclination().getRoll(), pinky.getInclination().getPitch(),

      // TO DO replace with different fingers
      pinky.getInclination().getYaw(), pinky.getAcc().getX(),
      pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(),
      pinky.getGyro().getY(), pinky.getGyro().getZ(),
      pinky.getInclination().getRoll(), pinky.getInclination().getPitch(),
      pinky.getInclination().getYaw(), pinky.getAcc().getX(),
      pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(),
      pinky.getGyro().getY(), pinky.getGyro().getZ(),
      pinky.getInclination().getRoll(), pinky.getInclination().getPitch(),
      pinky.getInclination().getYaw(), pinky.getAcc().getX(),
      pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(),
      pinky.getGyro().getY(), pinky.getGyro().getZ(),
      pinky.getInclination().getRoll(), pinky.getInclination().getPitch(),
      pinky.getInclination().getYaw(), pinky.getAcc().getX(),
      pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(),
      pinky.getGyro().getY(), pinky.getGyro().getZ(),
      pinky.getInclination().getRoll(), pinky.getInclination().getPitch(),
      pinky.getInclination().getYaw());

  if (size_written > size) {
    Serial.println("Error size is bigger than 512!!");
    Serial.print("size_written: ");
    Serial.println(size_written);
  }
}

bool GloveMeasurements::isNotComplete() {
  return this->imuSensorMeasurementMap_.size() < GloveMeasurements::MAP_SIZE;
}

GloveMeasurements::~GloveMeasurements() = default;
