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
    log_e("Error getting a finger measurement");
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
  /*
   ImuSensorMeasurement pinky = (this->getSensor(Finger::kPinky));
   ImuSensorMeasurement ring = (this->getSensor(Finger::kRing));
   ImuSensorMeasurement middle = (this->getSensor(Finger::kMiddle));
   ImuSensorMeasurement index = (this->getSensor(Finger::kIndex));
   ImuSensorMeasurement thumb = (this->getSensor(Finger::kThumb));
   */

  int size_written = sprintf(
      glove_measurement_buffer_,
      GloveMeasurements::kglove_mesurements_packet_format.c_str(), events_count,

      0.0, 0.0, 9.8, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,

      0.0, 0.0, 9.8, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,

      0.0, 0.0, 9.8, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,

      0.0, 0.0, 9.8, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,

      0.0, 0.0, 9.8, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3);

  if (size_written > size) {
    log_e("Error size is bigger than 512!!, size_written: %d", size_written);
  }
}

bool GloveMeasurements::isNotComplete() {
  return this->imuSensorMeasurementMap_.size() <
         GloveMeasurements::kImuSensorsAmount;
}

GloveMeasurements::~GloveMeasurements() = default;
