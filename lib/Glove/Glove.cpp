#include <Glove.h>

Glove::Glove() {
  this->chipId = this->getDeviceId();
  setUpSensors();
  Wire.begin(kSdaPin, kSclPin);
}

void Glove::init() {
  Serial.println(
      "\nType key when all sensors are placed over an horizontal plane:"
      "\n X = 0g, Y = 0g, Z = +1g orientation");
  while (!Serial.available()) {
    // Wait for a character.
  }
  calibrateSensors();
}

std::string Glove::getDeviceId() {
  uint8_t chipid[6];
  esp_efuse_mac_get_default(chipid);
  char chipIdString[17];
  snprintf(chipIdString, 17, "%02x:%02x:%02x:%02x:%02x:%02x\n", chipid[0],
           chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
  return chipIdString;
}

Glove::~Glove() {}

void Glove::setUpSensors() {
  for (Mpu mpu : sensors_) {
    mpu.setWriteMode();
  }
}

void Glove::calibrateSensors() {
  for (Mpu mpu : sensors_) {
    mpu.calibrate();
  }
}

std::map<Finger::Value, ImuSensorMeasurement> Glove::readSensors() {
  std::map<Finger::Value, ImuSensorMeasurement> measurements;
  for (Mpu mpu : sensors_) {
    ImuSensorMeasurement measurement = mpu.read();
    measurements[mpu.getFinger()] = measurement;
  }
  return measurements;
}
