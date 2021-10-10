#include <Glove.h>

const std::string Glove::kChipIdFormat = "%02x:%02x:%02x:%02x:%02x:%02x\n";
const int Glove::kChipIdSize = 6;
const int Glove::kChipIdStrSize = 17;
const int Glove::kSclPin = 22;
const int Glove::kSdaPin = 21;
const int Glove::kI2cSpeedHertz = 400000;

void Glove::init() {
  Wire.begin(kSdaPin, kSclPin);
  setUpSensors();
}

void Glove::setUpSensors() {
  for (auto sensor : sensors_) {
    sensor.second.setWriteMode();
  }
  for (auto sensor : sensors_) {
    sensor.second.init();
  }
}

void Glove::calibrateSensors() {
  digitalWrite(LED_BUILTIN, HIGH);
  for (auto sensor : sensors_) {
    sensor.second.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
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
  uint8_t chipid[Glove::kChipIdSize];
  esp_efuse_mac_get_default(chipid);
  char chipIdString[Glove::kChipIdStrSize];
  snprintf(chipIdString, Glove::kChipIdStrSize, Glove::kChipIdFormat.c_str(),
           chipid[0], chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
  return chipIdString;
}

const GloveSensors Glove::sensors_ = {
    {Finger::Value::kPinky, Mpu(Finger::Value::kPinky)},
    {Finger::Value::kRing, Mpu(Finger::Value::kRing)},
    {Finger::Value::kMiddle, Mpu(Finger::Value::kMiddle)},
    {Finger::Value::kIndex, Mpu(Finger::Value::kIndex)},
    {Finger::Value::kThumb, Mpu(Finger::Value::kThumb)}};

Glove::~Glove() = default;