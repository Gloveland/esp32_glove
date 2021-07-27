#include <Glove.h>

Glove::Glove() : pinkySensor(PINKY, "pinky"), thumbSensor(THUMB, "thumb") {
  this->chipId = this->getChipId();
  pinMode(PINKY, OUTPUT);
  digitalWrite(PINKY, HIGH);
  // pinMode(RING, OUTPUT);
  // digitalWrite(RING, HIGH);
  // pinMode(MIDDLE, OUTPUT);
  // digitalWrite(MIDDLE, HIGH);
  // pinMode(INDEX, OUTPUT);
  // digitalWrite(INDEX, HIGH);
  pinMode(THUMB, OUTPUT);
  digitalWrite(THUMB, HIGH);
  Wire.begin(I2C_SDA, I2C_SCL);  // Initialize  i2c bus comunication
}

void Glove::init() {
  Serial.println();
  Serial.print(
      "Type key when all sensors are placed over an horizontal plane:");
  Serial.println(" X = 0g, Y = 0g, Z = +1g orientation");
  while (!Serial.available()) {
    // wait for a character
  }
  // === Calibration === //
  pinkySensor.calibrate();
  // ringSensor.calibrate();
  // middleSensor.calibrate();
  // indexSensor.calibrate();
  thumbSensor.calibrate();
}

Movement Glove::readMovement(const int eventCount) {
  SensorMeasurement pinky = pinkySensor.read();
  SensorMeasurement thumb = thumbSensor.read();
  HandMov hand = {
      .pinky = pinky,
      .thumb = thumb,
  };
  Movement movement(eventCount, this->chipId, hand);
  return movement;
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
