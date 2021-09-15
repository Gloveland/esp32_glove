#include <GloveMeasurements.h>

const std::string GloveMeasurements::kglove_mesurements_packet_format =
    "%d\nP%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nR%.3f,%.3f,%.3f,%.3f,%."
    "3f,%.3f,%.3f,%.3f,%.3f\nM%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nI%."
    "3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nT%.3f,%.3f,%.3f,%.3f,%.3f,%."
    "3f,%.3f,%.3f,%.3f;";

GloveMeasurements::GloveMeasurements() {}

void GloveMeasurements::setSensorMeasurement(const Finger::Value &value,
                                             ImuSensorMeasurement measurement) {
  switch (value) {
    case Finger::kPinky:
      pinky_imu_measurement = &measurement;
      break;
    case Finger::kRing:
      ring_imu_measurement = &measurement;
      break;
    case Finger::kMiddle:
      middle_imu_measurement = &measurement;
      break;
    case Finger::kIndex:
      index_imu_measurement = &measurement;
      break;
    case Finger::kThumb:
      thumb_imu_measurement = &measurement;
      break;
    default:
      Serial.println(
          "[ERROR] Error setting a measurement in GloveMeasurements.");
  }
}

void GloveMeasurements::toPackage(int events_count,
                                  char *glove_measurement_buffer,
                                  const int size) {
  memset(glove_measurement_buffer, 0, size);

  if (this->isComplete()) {
    Serial.println("Glove measurements is not complete, skip package converion");
    return;
  }
  ImuSensorMeasurement pinky = *(this->pinky_imu_measurement);
  ImuSensorMeasurement ring = *(this->ring_imu_measurement);
  ImuSensorMeasurement middle = *(this->middle_imu_measurement);
  ImuSensorMeasurement index = *(this->index_imu_measurement);
  ImuSensorMeasurement thump = *(this->thumb_imu_measurement);

  int size_written = sprintf(
      glove_measurement_buffer,
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
  Serial.print("size_written: ");
  Serial.println(size_written);
  if (size_written > size) {
    Serial.print("Error size is bigger than 512!!");
  }
  Serial.println(String(glove_measurement_buffer));
}

bool GloveMeasurements::isComplete() {
  return (this->pinky_imu_measurement != nullptr);
}
GloveMeasurements::~GloveMeasurements() = default;