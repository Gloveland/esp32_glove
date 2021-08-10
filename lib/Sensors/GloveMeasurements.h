#ifndef GLOVE_MEASUREMENT_H_
#define GLOVE_MEASUREMENT_H_

struct GloveMeasurements {
  ImuSensorMeasurement thumb_imu_measurement{};
  ImuSensorMeasurement index_imu_measurement{};
  ImuSensorMeasurement middle_imu_measurement{};
  ImuSensorMeasurement ring_imu_measurement{};
  ImuSensorMeasurement pinky_imu_measurement{};

  ImuSensorMeasurement &getSensor(Finger::Value value) {
    switch (value) {
      case Finger::kPinky: return pinky_imu_measurement;
      case Finger::kRing: return ring_imu_measurement;
      case Finger::kMiddle: return middle_imu_measurement;
      case Finger::kIndex: return index_imu_measurement;
      case Finger::kThumb: return thumb_imu_measurement;
    }
  }

  void setSensorMeasurement(const Finger::Value &value,
                            ImuSensorMeasurement measurement) {

    switch (value) {
      case Finger::kPinky:pinky_imu_measurement = measurement;
        break;
      case Finger::kRing:ring_imu_measurement = measurement;
        break;
      case Finger::kMiddle:middle_imu_measurement = measurement;
        break;
      case Finger::kIndex:index_imu_measurement = measurement;
        break;
      case Finger::kThumb:thumb_imu_measurement = measurement;
        break;
      default:
        Serial.println(
            "[ERROR] Error setting a measurement in GloveMeasurements.");
    }
  }

  JSONVar toJson() const {
    JSONVar json;

    JSONVar pinky_json = pinky_imu_measurement.toJson();
    JSONVar thumb_json = thumb_imu_measurement.toJson();
    JSONVar ring_json = ring_imu_measurement.toJson();
    JSONVar index_json = index_imu_measurement.toJson();
    JSONVar middle_json = middle_imu_measurement.toJson();

    json[Finger::getName(Finger::kPinky).c_str()] = pinky_json;
    json[Finger::getName(Finger::kThumb).c_str()] = thumb_json;
    json[Finger::getName(Finger::kRing).c_str()] = ring_json;
    json[Finger::getName(Finger::kIndex).c_str()] = index_json;
    json[Finger::getName(Finger::kMiddle).c_str()] = middle_json;

    return json;
  }
};

#endif // GLOVE_MEASUREMENT_H_