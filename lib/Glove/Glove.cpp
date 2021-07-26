#include <Glove.h>

Glove::Glove() { this->chipId = this->getChipId(); }

void Glove::init() {
  pinMode(PINKY, OUTPUT);
  digitalWrite(PINKY, HIGH);
  // pinMode(RING, OUTPUT);
  // digitalWrite(RING, HIGH);
  // pinMode(MIDDLE, OUTPUT);
  // digitalWrite(MIDDLE, HIGH);
  // pinMode(INDEX, OUTPUT);
  // digitalWrite(INDEX, HIGH);
  pinMode(THUMP, OUTPUT);
  digitalWrite(THUMP, HIGH);

  Wire.begin(I2C_SDA, I2C_SCL);  // Initialize  i2c bus comunication
  pinkySensor.init(PINKY, "pinky");
  //
  thumpSensor.init(THUMP, "thump");
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
  thumpSensor.calibrate();
}

Movement Glove::readMovement(const int eventCount) {
  Finger pinky = pinkySensor.read();
  Finger thump = thumpSensor.read();
  Hand hand = {
      .pinky = pinky,
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