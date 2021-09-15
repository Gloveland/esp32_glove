#include <GloveMeasurements.h>

GloveMeasurements::GloveMeasurements(){}

void GloveMeasurements::setSensorMeasurement(const Finger::Value &value,
                            ImuSensorMeasurement measurement) {

    switch (value) {
      case Finger::kPinky:pinky_imu_measurement = &measurement;
        break;
      case Finger::kRing:ring_imu_measurement = &measurement;
        break;
      case Finger::kMiddle:middle_imu_measurement = &measurement;
        break;
      case Finger::kIndex:index_imu_measurement = &measurement;
        break;
      case Finger::kThumb:thumb_imu_measurement = &measurement;
        break;
      default:
        Serial.println(
            "[ERROR] Error setting a measurement in GloveMeasurements.");
    }
  }

  JSONVar GloveMeasurements::toJson() const {
    JSONVar json;

    JSONVar pinky_json = (*this->pinky_imu_measurement).toJson();
    JSONVar thumb_json = (*this->thumb_imu_measurement).toJson();
    JSONVar ring_json = (*this->ring_imu_measurement).toJson();
    JSONVar index_json = (*this->index_imu_measurement).toJson();
    JSONVar middle_json = (*this->middle_imu_measurement).toJson();

    json[Finger::getName(Finger::kPinky).c_str()] = pinky_json;
    json[Finger::getName(Finger::kThumb).c_str()] = thumb_json;
    json[Finger::getName(Finger::kRing).c_str()] = ring_json;
    json[Finger::getName(Finger::kIndex).c_str()] = index_json;
    json[Finger::getName(Finger::kMiddle).c_str()] = middle_json;

    return json;
  }

GloveMeasurements::~GloveMeasurements() = default;