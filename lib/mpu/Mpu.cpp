#include <Mpu.h>

float accX, accY, accZ;

Mpu::Mpu(const std::string name, const int ad0)
    : name(name),
      ad0(ad0),
      accRange(mpuAccRange::_2_G),
      gyroRange(mpuGyroRange::_250_DEG),
      accError(),
      gyroError(),
      previousGyro(),
      deviation(),
      inclinationfromAccError(),
      inclinationAngle(),
      previousTime(millis()) {}

void Mpu::beginCommunication() {
  this->checkAddress(mpuAddress::_OFF);
  digitalWrite(this->ad0, LOW);
  delay(100);
  this->checkAddress(mpuAddress::_ON);
}

void Mpu::endCommunication() {
  this->checkAddress(mpuAddress::_ON);
  digitalWrite(this->ad0, HIGH);
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
  this->accRange = accRange;
  this->gyroRange = gyroRange;
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
  Wire.write(this->accRange);  // ex 00010000 = 0x10  (+/- 8g full scale range)
  Wire.endTransmission(true);
  delay(20);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(GYRO_CONFIG_REGISTER);
  Wire.write(this->gyroRange);  // ex: 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(GENERAL_CONFIG);
  Wire.write(mpuBand::_21_HZ);  // ex: 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  this->endCommunication();
}

void Mpu::calibrate() {
  Serial.print("===================== Calibrating  ");
  Serial.print(this->name.c_str());
  Serial.println(" ====================");

  this->beginCommunication();

  float sumAccX = 0.0, sumAccY = 0.0, sumAccZ = 0.0;
  float accAngleX = 0.0, accAngleY = 0.0;
  float sumAngleFromAccX = 0.0, sumAngleFromAccY = 0.0;
  float sumGyroX = 0.0, sumGyroY = 0.0, sumGyroZ = 0.0;

  float minGyroX = FLT_MAX;
  float minGyroY = FLT_MAX;
  float minGyroZ = FLT_MAX;
  float maxGyroX = -FLT_MAX;
  float maxGyroY = -FLT_MAX;
  float maxGyroZ = -FLT_MAX;

  int times = 100.0;
  for (int i = 0; i <= times; i++) {
    RawMeasurement raw = this->readAllRaw();

    Acceleration acc = this->readAcc(raw.accX, raw.accY, raw.accZ, false);
    Gyro gyro = this->readGyro(raw.gyroX, raw.gyroY, raw.gyroZ, false);
    accAngleX = this->calculateAccAngleX(acc);
    accAngleY = this->calculateAccAngleY(acc);
    Serial.print("  ");
    Serial.print(accAngleX);
    Serial.print("  ");
    Serial.print(accAngleY);
    Serial.println(".");

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
  // Divide the sum by 200 to get the error value
  this->accError.x = sumAccX / times;
  this->accError.y = sumAccY / times;
  this->accError.z = sumAccZ / times;
  Serial.println("");
  Serial.print("AccErrorX: ");
  Serial.println(this->accError.x);
  Serial.print("AccErrorY: ");
  Serial.println(this->accError.y);
  Serial.print("AccErrorZ: ");
  Serial.println(this->accError.z);

  this->inclinationfromAccError.x = sumAngleFromAccX / times;
  this->inclinationfromAccError.y = sumAngleFromAccY / times;
  Serial.print("InclinationFromAccErrorX: ");
  Serial.println(this->inclinationfromAccError.x);
  Serial.print("InclinationFromAccErrorY: ");
  Serial.println(this->inclinationfromAccError.y);

  this->gyroError.x = sumGyroX / times;
  this->gyroError.y = sumGyroY / times;
  this->gyroError.z = sumGyroZ / times;

  Serial.println("");
  Serial.print("GyroErrorX: ");
  Serial.println(this->gyroError.x);
  Serial.print("GyroErrorY: ");
  Serial.println(this->gyroError.y);
  Serial.print("GyroErrorZ: ");
  Serial.println(this->gyroError.z);

  this->deviation.x = (maxGyroX - minGyroX) / 2.0;
  this->deviation.y = (maxGyroY - minGyroY) / 2.0;
  this->deviation.z = (maxGyroZ - minGyroZ) / 2.0;

  Serial.print("DeviationX: ");
  Serial.println(this->deviation.x);
  Serial.print("DeviationY: ");
  Serial.println(this->deviation.y);
  Serial.print("DeviationZ: ");
  Serial.println(this->deviation.z);
}

SensorMeasurement Mpu::read() {
  this->beginCommunication();
  const bool debug = true;
  float currentTime = millis();  // Divide by 1000 to get seconds
  float elapsedTime = (currentTime - this->previousTime) / 1000;
  this->previousTime = currentTime;
  RawMeasurement raw = this->readAllRaw();
  SensorMeasurement result;
  result.acc = this->readAcc(raw.accX, raw.accY, raw.accZ, debug);
  result.gyro = this->readGyro(raw.gyroX, raw.gyroY, raw.gyroZ, debug);
  result.inclination =
      this->readInclination(result.acc, result.gyro, elapsedTime, debug);
  result.temperature = this->readTemperature(raw.temp);
  this->endCommunication();
  return result;
}

RawMeasurement Mpu::readAllRaw() {
  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(ACCEL_XOUT_H);  // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  // read 14 registers: | ax | ay | az | temp | gx | gy | gz |(each value is
  // stored in 2 registers)
  Wire.requestFrom(mpuAddress::_ON, ALL_REGISTERS, true);

  RawMeasurement rawMeasurement;
  rawMeasurement.accX =
      (Wire.read() << BITS_IN_BYTE | Wire.read());  // X-axis value
  rawMeasurement.accY =
      (Wire.read() << BITS_IN_BYTE | Wire.read());  // Y-axis value
  rawMeasurement.accZ =
      (Wire.read() << BITS_IN_BYTE | Wire.read());  // Z-axis value

  rawMeasurement.temp = (Wire.read() << BITS_IN_BYTE | Wire.read());

  rawMeasurement.gyroX = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.gyroY = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.gyroZ = (Wire.read() << BITS_IN_BYTE | Wire.read());
  return rawMeasurement;
}

float Mpu::getAccScale(const mpuAccRange accRange) {
  switch (accRange) {
    case mpuAccRange::_16_G:
      return (float)mpuAccScale::_16;
    case mpuAccRange::_8_G:
      return (float)mpuAccScale::_8;
    case mpuAccRange::_4_G:
      return (float)mpuAccScale::_4;
    default:
      return (float)mpuAccScale::_2;
  }
}
float Mpu::getGyroScale(const mpuGyroRange gyroRange) {
  switch (gyroRange) {
    case mpuGyroRange::_2000_DEG:
      return (float)mpuGyroScaleX10::_2000 / GYRO_SCALE_DIVISOR;
    case mpuGyroRange::_1000_DEG:
      return (float)mpuGyroScaleX10::_1000 / GYRO_SCALE_DIVISOR;
    case mpuGyroRange::_500_DEG:
      return (float)mpuGyroScaleX10::_500 / GYRO_SCALE_DIVISOR;
    default:
      return (float)mpuGyroScaleX10::_250 / GYRO_SCALE_DIVISOR;
  }
}

Acceleration Mpu::readAcc(const int16_t rawAccX, const int16_t rawAccY,
                          const int16_t rawAccZ, const bool debug) {
  Acceleration acc;
  float accScale = this->getAccScale(this->accRange);
  acc.x = ((float)rawAccX / accScale) * GRAVITY_EARTH - this->accError.x;
  acc.y = ((float)rawAccY / accScale) * GRAVITY_EARTH - this->accError.y;
  acc.z = ((float)rawAccZ / accScale) * GRAVITY_EARTH - this->accError.z;

  if (debug) {
    Serial.print(name.c_str());
    Serial.print("   aX: ");
    Serial.print(acc.x);
    Serial.print("   aY: ");
    Serial.print(acc.y);
    Serial.print("   aZ: ");
    Serial.print(acc.z);
    Serial.print(",");
    Serial.print("   m/(seg)^2    ");
  }
  return acc;
}

float Mpu::readTemperature(const int16_t rawTemp) {
  float temperature = (rawTemp / 340.0) + 36.53;
  return temperature;
}

Gyro Mpu::readGyro(const int16_t rawGyroX, const int16_t rawGyroY,
                   const int16_t rawGyroZ, const bool debug) {
  float gyroScale = this->getGyroScale(this->gyroRange);
  float gyroX = ((float)rawGyroX / gyroScale);
  float gyroY = ((float)rawGyroY / gyroScale);
  float gyroZ = ((float)rawGyroZ / gyroScale);

  Gyro gyro;
  // Si la diferencia absoluta entre esta medicion y la anterior es menor a la
  // desviacion maxima lo desestimo.
  if (abs(this->previousGyro.x - gyroX) > this->deviation.x) {
    gyro.x = gyroX - this->gyroError.x;
  }
  this->previousGyro.x = gyroX;

  if (abs(this->previousGyro.y - gyroY) > this->deviation.y) {
    gyro.y = gyroY - this->gyroError.y;
  }
  this->previousGyro.y = gyroY;

  if (abs(this->previousGyro.z - gyroZ) > this->deviation.z) {
    gyro.z = gyroZ - this->gyroError.z;
  }
  this->previousGyro.z = gyroZ;

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

  return gyro;
}

Inclination Mpu::readInclination(const Acceleration acc, const Gyro gyro,
                                 const float elapsedTime, const bool debug) {
  // Currently the raw values are in degrees per seconds, deg/s, so we need to
  // multiply by sendonds (s) to get the angle in degrees,  deg/s * s = deg
  this->inclinationAngle.x = this->inclinationAngle.x + gyro.x * elapsedTime;
  this->inclinationAngle.y = this->inclinationAngle.y + gyro.y * elapsedTime;
  this->inclinationAngle.z = this->inclinationAngle.z + gyro.z * elapsedTime;
  /*
  Serial.print("   degX: ");Serial.print(this->inclinationAngle.x);
  Serial.print("   degY: ");Serial.print(this->inclinationAngle.y);
  Serial.print("   degZ: ");Serial.print(this->inclinationAngle.z);
  */
  // Calculating Roll and Pitch from the accelerometer data
  float inclinationfromAccX =
      this->calculateAccAngleX(acc) - this->inclinationfromAccError.x;
  float inclinationfromAccY =
      this->calculateAccAngleY(acc) - this->inclinationfromAccError.y;
  // Serial.print("   accAngleX: ");Serial.print(accAngleX );
  // Serial.print("   accAngleY: ");Serial.print(accAngleY );

  // ajustamos el angulo de inclinacion con la aceleracion
  if (abs(inclinationfromAccX - this->inclinationAngle.x) > 5.0) {
    this->inclinationAngle.x = inclinationfromAccX;
  }
  if (abs(inclinationfromAccY - this->inclinationAngle.y) > 5.0) {
    this->inclinationAngle.y = inclinationfromAccY;
  }
  Inclination inclination;
  // Complementary filter - combine acceleromter and gyro angle values
  inclination.roll = this->inclinationAngle.x;
  inclination.pitch = this->inclinationAngle.y;
  inclination.yaw = this->inclinationAngle.z;

  // Print the values on the serial monitor
  if (debug) {
    Serial.print("  roll:");
    Serial.print(inclination.roll);
    Serial.print("  pitch:");
    Serial.print(inclination.pitch);
    Serial.print("  yaw:");
    Serial.print(inclination.yaw);
  }
  return inclination;
}

float Mpu::calculateAccAngleX(const Acceleration acc) {
  return (atan(acc.y / sqrt(pow(acc.x, 2) + pow(acc.z, 2))) * 180 / PI);
}

float Mpu::calculateAccAngleY(const Acceleration acc) {
  return (atan(-1 * acc.x / sqrt(pow(acc.y, 2) + pow(acc.z, 2))) * 180 / PI);
}

Mpu::~Mpu() {}
