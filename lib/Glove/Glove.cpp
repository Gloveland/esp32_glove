#include <Glove.h>

Glove::Glove() { this->chipId = this->getChipId(); }

void Glove::init() {
  // pinkySensor.init(I2C_SDA_PINKY, I2C_SCL_PINKY);
  // ringSensor.init(I2C_SDA_RING, I2C_SCL_RING);
  // middleSensor.init(I2C_SDA_MIDDLE, I2C_SCL_MIDDLE);
  // indexSensor.init(I2C_SDA_INDEX, I2C_SCL_INDEX);
  thumpSensor.init(I2C_SDA_THUMB, I2C_SCL_THUMB);

  Serial.println();
  Serial.println(
      "Type key when all sensors are placed over an horizontal plane:"
      " X = 0g, Y = 0g, Z = +1g orientation");
  while (!Serial.available()) {
    // wait for a character
  }
  // === Calibration === //
  // pinkySensor.calibrate();
  // ringSensor.calibrate();
  // middleSensor.calibrate();
  // indexSensor.calibrate();
  thumpSensor.calibrate();
}

Movement Glove::readMovement(const int eventCount) {
  Finger thump = thumpSensor.read();
  Hand hand = {
      .thump = thump,
  };
  Movement mov(eventCount, this->chipId, hand);
  return mov;
}

string Glove::getChipId() {
  uint8_t chipid[6];
  esp_efuse_mac_get_default(chipid);
  char chipIdString[17];
  snprintf(chipIdString, 17, "%02x:%02x:%02x:%02x:%02x:%02x\n", chipid[0],
           chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
  return chipIdString;
}

Glove::~Glove() {}