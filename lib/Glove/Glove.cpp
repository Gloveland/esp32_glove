#include <Glove.h>

const std::string Glove::kChipIdFormat = "%02x:%02x:%02x:%02x:%02x:%02x\n";
const int Glove::kChipIdSize = 6;
const int Glove::kChipIdStrSize = 17;
const int Glove::kSclPin = 22;
const int Glove::kSdaPin = 21;
const int Glove::OK = 0;
const int Glove::DATA_BUFFER_ERROR = 1;
const int Glove::NACK_ERROR = 2;
const int Glove::UNKNOWN_ERROR = 4;
const int Glove::kI2cSpeedHertz = 400000;

Glove::Glove(): sensor_iterator_( Glove::sensors_.begin()){

}

void Glove::init() {
  Wire.begin(kSdaPin, kSclPin);
  setUpSensors();
}

void Glove::setUpSensors() {
  for (auto sensor : sensors_) {
    sensor.second.setWriteMode();
  }
  assert(this->checkAddress(mpuAddress::_OFF));
  for (auto sensor : sensors_) {
    sensor.second.init();
  }
}

bool Glove::checkAddress(int address) {
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error != Glove::OK) {
    log_e("Error n %d checking address: 0x%X ", error, address);
    if (error == Glove::DATA_BUFFER_ERROR) {
      log_e(" Data too long to fit in transmit buffer. ");
      log_e(" Is i2c bus initialize?");
    }
    if (error == Glove::NACK_ERROR) {
      log_e("received NACK on transmit of address");
    }
    if (error == Glove::UNKNOWN_ERROR) {
      log_e(" Unknow error");
    }
    return false;
  }
  return true;
}

void Glove::calibrateSensors() {
  digitalWrite(LED_BUILTIN, HIGH);
  for (auto sensor : sensors_) {
    sensor.second.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
}

ImuSensorMeasurement Glove::readNextSensor(float elapsedTime) {
  if (this->sensor_iterator_ == sensors_.end()) {
    this->sensor_iterator_ = sensors_.begin();
  }
  Finger::Value finger = this->sensor_iterator_->first;
  Mpu sensor = this->sensor_iterator_->second;
  ImuSensorMeasurement measurementRead = sensor.read();
  this->sensor_iterator_++;
  return measurementRead;
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