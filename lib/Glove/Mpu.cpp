#include <Mpu.h>

Mpu::Mpu(const Finger::Value &finger) :
    finger_(finger),
    ad0_(Finger::getAd0Pin(finger)),
    accRange_(mpuAccRange::_2_G),
    gyroRange_(mpuGyroRange::_250_DEG),
    accError_(),
    gyroError_(),
    previousGyro_(),
    deviation_(),
    previousTime_(millis()) {
}

void Mpu::beginCommunication() {
  this->checkAddress(mpuAddress::_OFF);
  digitalWrite(this->ad0_, LOW);
  delay(100);
  this->checkAddress(mpuAddress::_ON);
}

void Mpu::endCommunication() {
  this->checkAddress(mpuAddress::_ON);
  digitalWrite(this->ad0_, HIGH);
  delay(100);
  this->checkAddress(mpuAddress::_OFF);
}

void Mpu::checkAddress(int address) {
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.print("Error checking address: ");
    Serial.print(" 0x");
    if (address < 16) {
      Serial.print("0");
    }
    Serial.println(address, HEX);
    if (error == 1) {
      Serial.print(" Data too long to fit in transmit buffer. ");
      Serial.println(" Is i2c bus initialize?");
    }
    if (error == 4) {
      Serial.println(" Unknow error");
    }
  }
  assert(error == 0);
}
void Mpu::init(const mpuAccRange accRange, const mpuGyroRange gyroRange) {
  this->accRange_ = accRange;
  this->gyroRange_ = gyroRange;
  this->init();
}

void Mpu::init() {
  this->beginCommunication();

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(PWR_MGMT_1);
  Wire.write(RESET);
  Wire.endTransmission(true);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(ACC_CONFIG_REGISTER);
  Wire.write(this->accRange_);  // ex 00010000 = 0x10  (+/- 8g full scale range)
  Wire.endTransmission(true);
  delay(20);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(GYRO_CONFIG_REGISTER);
  Wire.write(this->gyroRange_);  // ex: 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(GENERAL_CONFIG);
  Wire.write(mpuBand::_21_HZ);  // ex: 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  this->endCommunication();
}

/**
 * The calibration consist in readings raw measurements values when the sensor
 * is static in horizontal plane. The expected values for acceleration are 0.0
 * in x, 0.0 in y and 9.8 in z. The error is the difference between the expected
 * value and the value read. We sum all the errors and divede by the amount of
 * measurement reads to get the average error in each axis. The same is done
 * with de angle and the gyroscope readings The expected value for the angular
 * velocity is 0.0 for each axis. We calculate the maximum and the minimum value
 * read to get the deviation This way we can  avoid noise in future
 * measurements.
 */
void Mpu::calibrate() {
  Serial.print("===================== Calibrating  ");
  Serial.print(Finger::getName(this->finger_).c_str());
  Serial.println(" ====================");

  this->beginCommunication();

  float sumAccX = 0.0, sumAccY = 0.0, sumAccZ = 0.0;
  float accAngleX = 0.0, accAngleY = 0.0;
  float sumAngleFromAccX = 0.0, sumAngleFromAccY = 0.0;
  float sumGyroX = 0.0, sumGyroY = 0.0, sumGyroZ = 0.0;

  float minGyroX = FLT_MAX, minGyroY = FLT_MAX, minGyroZ = FLT_MAX;
  float maxGyroX = -FLT_MAX, maxGyroY = -FLT_MAX, maxGyroZ = -FLT_MAX;

  int times = 100.0;
  for (int i = 0; i <= times; i++) {
    RawMeasurement raw = this->readAllRaw();

    Acceleration acc = this->readAcc(raw.accX, raw.accY, raw.accZ, false);
    Gyro gyro = this->readGyro(raw.gyroX, raw.gyroY, raw.gyroZ, false);
    accAngleX = this->calculateAccAngleX(acc);
    accAngleY = this->calculateAccAngleY(acc);

    sumAccX += acc.x;
    sumAccY += acc.y;
    sumAccZ += (acc.z - GRAVITY_EARTH);
    sumAngleFromAccX += accAngleX;
    sumAngleFromAccY += accAngleY;

    if (gyro.x < minGyroX) {
      minGyroX = gyro.x;
    }
    if (gyro.y < minGyroY) {
      minGyroY = gyro.y;
    }
    if (gyro.z < minGyroZ) {
      minGyroZ = gyro.z;
    }
    if (gyro.x > maxGyroX) {
      maxGyroX = gyro.x;
    }
    if (gyro.y > maxGyroY) {
      maxGyroY = gyro.y;
    }
    if (gyro.z > maxGyroZ) {
      maxGyroZ = gyro.z;
    }
    sumGyroX += gyro.x;
    sumGyroY += gyro.y;
    sumGyroZ += gyro.z;
    delay(20);
  }
  this->endCommunication();
  this->setAccelerationError(times, sumAccX, sumAccY, sumAccZ);
  this->logAccelerationError();

  this->inclination_.setInclinationError(times, sumAngleFromAccX, sumAngleFromAccY);

  this->setGyroError(times, sumGyroX, sumGyroY, sumGyroZ);
  this->logGyroError();

  this->setDeviation(times, maxGyroX, maxGyroY, maxGyroZ, minGyroX, minGyroY,
                     minGyroZ);
  this->logDeviation();
}

void Mpu::setAccelerationError(int times, float sumAccX, float sumAccY,
                               float sumAccZ) {
  this->accError_.x = sumAccX / times;
  this->accError_.y = sumAccY / times;
  this->accError_.z = sumAccZ / times;
}

void Mpu::logAccelerationError() {
  Serial.println("");
  Serial.print("AccErrorX: ");
  Serial.println(this->accError_.x);
  Serial.print("AccErrorY: ");
  Serial.println(this->accError_.y);
  Serial.print("AccErrorZ: ");
  Serial.println(this->accError_.z);
}

void Mpu::setGyroError(int times, float sumGyroX, float sumGyroY,
                       float sumGyroZ) {
  this->gyroError_.x = sumGyroX / times;
  this->gyroError_.y = sumGyroY / times;
  this->gyroError_.z = sumGyroZ / times;
}

void Mpu::logGyroError() {
  Serial.println("");
  Serial.print("GyroErrorX: ");
  Serial.println(this->gyroError_.x);
  Serial.print("GyroErrorY: ");
  Serial.println(this->gyroError_.y);
  Serial.print("GyroErrorZ: ");
  Serial.println(this->gyroError_.z);
}

void Mpu::setDeviation(int times, float maxX, float maxY, float maxZ,
                       float minX, float minY, float minZ) {
  this->deviation_.x = (maxX - minX) / 2.0;
  this->deviation_.y = (maxY - minY) / 2.0;
  this->deviation_.z = (maxZ - minZ) / 2.0;
}

void Mpu::logDeviation() {
  Serial.print("DeviationX: ");
  Serial.println(this->deviation_.x);
  Serial.print("DeviationY: ");
  Serial.println(this->deviation_.y);
  Serial.print("DeviationZ: ");
  Serial.println(this->deviation_.z);
}

ImuSensorMeasurement Mpu::read() {
  this->beginCommunication();
  const bool debug = true;
  float currentTime = millis();  // Divide by 1000 to get seconds
  float elapsedTime = (currentTime - this->previousTime_) / 1000;
  this->previousTime_ = currentTime;
  RawMeasurement raw = this->readAllRaw();
  ImuSensorMeasurement result;
  result.acc = this->readAcc(raw.accX, raw.accY, raw.accZ, debug);
  result.gyro = this->readGyro(raw.gyroX, raw.gyroY, raw.gyroZ, debug);
  result.inclination =
      inclination_.computeInclination(result.acc, result.gyro, elapsedTime);
  result.temperature = this->readTemperature(raw.temp);
  this->endCommunication();
  return result;
}

/**
 * We read 14 registers from mpu (each value is stored in 2 registers)
 * the registers order is: | ax | ay | az | temp | gx | gy | gz |
 */
RawMeasurement Mpu::readAllRaw() {
  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(mpuAddress::_ON, ALL_REGISTERS, true);

  RawMeasurement rawMeasurement;
  rawMeasurement.accX = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.accY = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.accZ = (Wire.read() << BITS_IN_BYTE | Wire.read());

  rawMeasurement.temp = (Wire.read() << BITS_IN_BYTE | Wire.read());

  rawMeasurement.gyroX = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.gyroY = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.gyroZ = (Wire.read() << BITS_IN_BYTE | Wire.read());
  return rawMeasurement;
}
float Mpu::getAccScale(const mpuAccRange accRange) {
  switch (accRange) {
    case mpuAccRange::_16_G:return (float) mpuAccScale::_16;
    case mpuAccRange::_8_G:return (float) mpuAccScale::_8;
    case mpuAccRange::_4_G:return (float) mpuAccScale::_4;
    default:return (float) mpuAccScale::_2;
  }
}

float Mpu::getGyroScale(const mpuGyroRange gyroRange) {
  switch (gyroRange) {
    case mpuGyroRange::_2000_DEG:
      return (float) mpuGyroScaleX10::_2000 / GYRO_SCALE_DIVISOR;
    case mpuGyroRange::_1000_DEG:
      return (float) mpuGyroScaleX10::_1000 / GYRO_SCALE_DIVISOR;
    case mpuGyroRange::_500_DEG:
      return (float) mpuGyroScaleX10::_500 / GYRO_SCALE_DIVISOR;
    default:return (float) mpuGyroScaleX10::_250 / GYRO_SCALE_DIVISOR;
  }
}

Acceleration Mpu::readAcc(const int16_t rawAccX, const int16_t rawAccY,
                          const int16_t rawAccZ, const bool debug) {
  Acceleration acc;
  float accScale = this->getAccScale(this->accRange_);
  acc.x = ((float) rawAccX / accScale) * GRAVITY_EARTH - this->accError_.x;
  acc.y = ((float) rawAccY / accScale) * GRAVITY_EARTH - this->accError_.y;
  acc.z = ((float) rawAccZ / accScale) * GRAVITY_EARTH - this->accError_.z;

//  this->log(debug, acc);
  return acc;
}

void Mpu::log(const bool debug, Acceleration acc) {
  if (debug) {
    Serial.print(Finger::getName(this->finger_).c_str());
    Serial.print("   aX: ");
    Serial.print(acc.x);
    Serial.print("   aY: ");
    Serial.print(acc.y);
    Serial.print("   aZ: ");
    Serial.print(acc.z);
    Serial.print(",");
    Serial.print("   m/(seg)^2    ");
  }
}

float Mpu::readTemperature(const int16_t rawTemp) {
  float temperature = (rawTemp / 340.0) + 36.53;
  return temperature;
}

/**
 * Get angular velocity of rotation from raw gyro readings.
 * If the difference between the measurement and the previous mesurement
 * is lower than the maximum deviation we dismiss the measurement because is
 * considered noise
 */
Gyro Mpu::readGyro(const int16_t rawGyroX, const int16_t rawGyroY,
                   const int16_t rawGyroZ, const bool debug) {
  float gyroScale = this->getGyroScale(this->gyroRange_);
  float gyroX = ((float) rawGyroX / gyroScale);
  float gyroY = ((float) rawGyroY / gyroScale);
  float gyroZ = ((float) rawGyroZ / gyroScale);

  Gyro gyro;
  if (abs(this->previousGyro_.x - gyroX) > this->deviation_.x) {
    gyro.x = gyroX - this->gyroError_.x;
  }
  this->previousGyro_.x = gyroX;

  if (abs(this->previousGyro_.y - gyroY) > this->deviation_.y) {
    gyro.y = gyroY - this->gyroError_.y;
  }
  this->previousGyro_.y = gyroY;

  if (abs(this->previousGyro_.z - gyroZ) > this->deviation_.z) {
    gyro.z = gyroZ - this->gyroError_.z;
  }
  this->previousGyro_.z = gyroZ;
//  this->log(debug, gyro);
  return gyro;
}

void Mpu::log(const bool debug, const Gyro gyro) {
  if (debug) {
    Serial.print("   gX: ");
    Serial.print(gyro.x);
    Serial.print(",");
    Serial.print("   gY: ");
    Serial.print(gyro.y);
    Serial.print(",");
    Serial.print("   gZ: ");
    Serial.print(gyro.z);
    Serial.print(",");
    Serial.print("   degrees/seg         ");
  }
}

void Mpu::log(const float accAngleX, const float accAngleY) {
  Serial.print("   accAngleX: ");
  Serial.print(accAngleX);
  Serial.print("   accAngleY: ");
  Serial.print(accAngleY);
}

void Mpu::log(const bool debug, const Inclination inclination) {
  if (debug) {
    Serial.print("  roll:");
    Serial.print(inclination.roll);
    Serial.print("  pitch:");
    Serial.print(inclination.pitch);
    Serial.print("  yaw:");
    Serial.print(inclination.yaw);
  }
}

float Mpu::calculateAccAngleX(const Acceleration acc) {
  return (atan(acc.y / sqrt(pow(acc.x, 2) + pow(acc.z, 2))) * 180 / PI);
}

float Mpu::calculateAccAngleY(const Acceleration acc) {
  return (atan(-1 * acc.x / sqrt(pow(acc.y, 2) + pow(acc.z, 2))) * 180 / PI);
}

void Mpu::setWriteMode() {
  pinMode(this->ad0_, OUTPUT);
  digitalWrite(this->ad0_, HIGH);
}

Finger::Value Mpu::getFinger() {
  return this->finger_;
}

Mpu::~Mpu() {}
