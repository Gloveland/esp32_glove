#include <Glove.h>

const std::string Glove::kchip_id_format = "%02x:%02x:%02x:%02x:%02x:%02x\n";
const int Glove::kchip_id_size = 6;
const int Glove::kchip_id_str_size = 17;


void Glove::init() {
  Wire.begin(kSdaPin, kSclPin);
  setUpSensors();
}

void Glove::setUpSensors() {
  for (auto sensor : sensors_) {
    sensor.second.setWriteMode();
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
  GloveMeasurements measurements;
  for (auto sensor : sensors_) {
    ImuSensorMeasurement measurement = sensor.second.read();
    measurements.setSensorMeasurement(sensor.first, measurement);
  }
  return measurements;
}

std::string Glove::getDeviceId() {
    uint8_t chipid[Glove::kchip_id_size];
    esp_efuse_mac_get_default(chipid);
    char chipIdString[Glove::kchip_id_str_size];
    snprintf(chipIdString, Glove::kchip_id_str_size, Glove::kchip_id_format.c_str(), chipid[0],
             chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
    return chipIdString;
  }

const GloveSensors Glove::sensors_ = {
    {Finger::Value::kPinky, Mpu(Finger::Value::kPinky)}  //,
    //{Finger::Value::kThumb, Mpu(Finger::Value::kThumb, false)}
};

Glove::~Glove() = default;