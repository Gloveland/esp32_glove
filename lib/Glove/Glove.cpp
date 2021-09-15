#include <Glove.h>

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
  GloveMeasurements measurements;
  for (auto sensor : sensors_) {
    ImuSensorMeasurement measurement = sensor.second.read();
    measurements.setSensorMeasurement(sensor.first, measurement);
  }
  return measurements;
}

std::string Glove::getDeviceId() {
    uint8_t chipid[6];
    esp_efuse_mac_get_default(chipid);
    char chipIdString[17];
    snprintf(chipIdString, 17, "%02x:%02x:%02x:%02x:%02x:%02x\n", chipid[0],
             chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
    return chipIdString;
  }

const Glove::GloveSensors sensors_ = {
    {Finger::Value::kPinky, Mpu(Finger::Value::kPinky)}  //,
    //{Finger::Value::kThumb, Mpu(Finger::Value::kThumb)}
};

Glove::~Glove() = default;