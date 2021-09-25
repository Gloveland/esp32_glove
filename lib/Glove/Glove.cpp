#include <Glove.h>

const std::string Glove::kchip_id_format = "%02x:%02x:%02x:%02x:%02x:%02x\n";
const int Glove::kchip_id_size = 6;
const int Glove::kchip_id_str_size = 17;

void Glove::init() {
  Wire.begin(kSdaPin, kSclPin);
  setUpSensors();
  Serial.println(
      "\nType key when all sensors are placed over an horizontal plane:"
      "\n X = 0g, Y = 0g, Z = +1g orientation");
  while (!Serial.available()) {
    // Wait for a character.
  }
  calibrateSensors();
}

void Glove::setUpSensors() {
  for (auto sensor : sensors_) {
    sensor.second.setWriteMode();
    sensor.second.init();
  }
}

void Glove::calibrateSensors() {
  for (auto sensor : sensors_) {
    sensor.second.calibrate();
  }
}

GloveMeasurements Glove::readSensors() {
  std::map<const Finger::Value, ImuSensorMeasurement> measurementsMap;
  for (auto sensor : sensors_) {
    ImuSensorMeasurement measurement = sensor.second.read();
    measurementsMap.insert(std::pair<Finger::Value, ImuSensorMeasurement>(
        sensor.first, measurement));
  }
  GloveMeasurements glove_measurements;
  glove_measurements.setSensorMeasurementsMap(measurementsMap);
  return glove_measurements;
}

std::string Glove::getDeviceId() {
  uint8_t chipid[Glove::kchip_id_size];
  esp_efuse_mac_get_default(chipid);
  char chipIdString[Glove::kchip_id_str_size];
  snprintf(chipIdString, Glove::kchip_id_str_size,
           Glove::kchip_id_format.c_str(), chipid[0], chipid[1], chipid[2],
           chipid[3], chipid[4], chipid[5]);
  return chipIdString;
}

const GloveSensors Glove::sensors_ = {
    {Finger::Value::kPinky, Mpu(Finger::Value::kPinky)}  //,
    //{Finger::Value::kThumb, Mpu(Finger::Value::kThumb, false)}
};

Glove::~Glove() = default;