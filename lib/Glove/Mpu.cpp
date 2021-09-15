#include <Mpu.h>

Mpu::Mpu(const Finger::Value &finger) :
    finger_(finger),
    ad0_(Finger::getAd0Pin(finger)),
    accelerometer(Accelerometer(Range::_2_G, true)),
    inclination_calculator (InclinationCalculator(true)),
    gyroRange_(mpuGyroRange::_250_DEG),
    gyroError_(Gyro(0,0,0)),
    previousGyro_(Gyro(0,0,0)),
    deviation_(Gyro(0,0,0)),
    previousTime_(millis()) {}

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

void Mpu::init() {
  this->beginCommunication();

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(PWR_MGMT_1);
  Wire.write(RESET);
  Wire.endTransmission(true);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(ACC_CONFIG_REGISTER);
  Wire.write(this->accelerometer.getRange());  // ex 00010000 = 0x10  (+/- 8g full scale range)
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

  float sum_acc_x = 0.0, sum_acc_y = 0.0, sum_acc_z = 0.0;
  float accAngleX = 0.0, accAngleY = 0.0;
  float sumAngleFromAccX = 0.0, sumAngleFromAccY = 0.0;
  float sumGyroX = 0.0, sumGyroY = 0.0, sumGyroZ = 0.0;

  float minGyroX = FLT_MAX, minGyroY = FLT_MAX, minGyroZ = FLT_MAX;
  float maxGyroX = -FLT_MAX, maxGyroY = -FLT_MAX, maxGyroZ = -FLT_MAX;

  int times = 100.0;
  for (int i = 0; i <= times; i++) {
    RawMeasurement raw = this->readAllRaw();

    Acceleration acc = this->accelerometer.readAcc(raw.accX, raw.accY, raw.accZ);
    Gyro gyro = this->readGyro(raw.gyroX, raw.gyroY, raw.gyroZ, false);
    accAngleX = acc.calculateAngleX();
    accAngleY = acc.calculateAngleY();

    sum_acc_x += acc.getX();
    sum_acc_y += acc.getY();
    sum_acc_z += (acc.getZ() - this->GRAVITY_EARTH);
    sumAngleFromAccX += accAngleX;
    sumAngleFromAccY += accAngleY;

    if (gyro.getX() < minGyroX) {
      minGyroX = gyro.getX();
    }
    if (gyro.getY() < minGyroY) {
      minGyroY = gyro.getY();
    }
    if (gyro.getZ() < minGyroZ) {
      minGyroZ = gyro.getZ();
    }
    if (gyro.getX() > maxGyroX) {
      maxGyroX = gyro.getX();
    }
    if (gyro.getY() > maxGyroY) {
      maxGyroY = gyro.getY();
    }
    if (gyro.getZ() > maxGyroZ) {
      maxGyroZ = gyro.getZ();
    }
    sumGyroX += gyro.getX();
    sumGyroY += gyro.getY();
    sumGyroZ += gyro.getZ();
    delay(20);
  }
  this->endCommunication();
  this->accelerometer.calibrate(times, sum_acc_x, sum_acc_y, sum_acc_z);

  this->inclination_calculator.setError(times, sumAngleFromAccX, sumAngleFromAccY);

  this->setGyroError(times, sumGyroX, sumGyroY, sumGyroZ);
  this->logGyroError();

  this->setDeviation(times, maxGyroX, maxGyroY, maxGyroZ, minGyroX, minGyroY,
                     minGyroZ);
  this->logDeviation();
}


void Mpu::setGyroError(int times, float sumGyroX, float sumGyroY,
                       float sumGyroZ) {
  float x = sumGyroX / times;
  float y  = sumGyroY / times;
  float z = sumGyroZ / times;
  this->gyroError_ = Gyro(x, y, z);
}

void Mpu::logGyroError() {
  Serial.println("");
  Serial.print("GyroErrorX: ");
  Serial.println(this->gyroError_.getX());
  Serial.print("GyroErrorY: ");
  Serial.println(this->gyroError_.getY());
  Serial.print("GyroErrorZ: ");
  Serial.println(this->gyroError_.getZ());
}

void Mpu::setDeviation(int times, float maxX, float maxY, float maxZ,
                       float minX, float minY, float minZ) {
  float x = (maxX - minX) / 2.0;
  float y  = (maxY - minY) / 2.0;
  float z = (maxZ - minZ) / 2.0;
  this->gyroError_ = Gyro(x, y, z);
}

void Mpu::logDeviation() {
  Serial.print("DeviationX: ");
  Serial.println(this->deviation_.getX());
  Serial.print("DeviationY: ");
  Serial.println(this->deviation_.getY());
  Serial.print("DeviationZ: ");
  Serial.println(this->deviation_.getZ());
}

ImuSensorMeasurement Mpu::read() {
  this->beginCommunication();
  const bool debug = true;
  float currentTime = millis();  // Divide by 1000 to get seconds
  float elapsedTime = (currentTime - this->previousTime_) / 1000;
  this->previousTime_ = currentTime;
  RawMeasurement raw = this->readAllRaw();
  this->endCommunication();

  Acceleration acc = this->accelerometer.readAcc(raw.accX, raw.accY, raw.accZ, debug);
  Gyro gyro = this->readGyro(raw.gyroX, raw.gyroY, raw.gyroZ, debug);
  Inclination inclination = this->inclination_calculator.calculateInclination(result.acc, result.gyro, elapsedTime);
  float temperature = this->readTemperature(raw.temp);
  ImuSensorMeasurement result = ImuSensorMeasurement(acc, gyro, inclination);
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


void Mpu::log(const bool debug, Acceleration acc) {
  if (debug) {
    Serial.print(Finger::getName(this->finger_).c_str());
    acc.log();
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
  float gyro_x = ((float) rawGyroX / gyroScale);
  float gyro_y = ((float) rawGyroY / gyroScale);
  float gyro_z= ((float) rawGyroZ / gyroScale);

  float x_value, y_value, z_value = 0.0;
  if (abs(this->previousGyro_.getX() - gyro_x) > this->deviation_.getX()) {
    x_value = gyro_x - this->gyroError_.getX();
  }

  if (abs(this->previousGyro_.getY() - gyro_y) > this->deviation_.getY()) {
    y_value = gyro_y - this->gyroError_.getY();
  }

  if (abs(this->previousGyro_.getZ() - gyro_z) > this->deviation_.getZ()) {
    z_value= gyro_z - this->gyroError_.getZ();
  }
  this->previousGyro_ = Gyro(gyro_x, gyro_y, gyro_z);
 
  return Gyro(x_value, y_value, z_value);
}

void Mpu::log(const bool debug, const Gyro gyro) {
  if (debug) {
    Serial.print("   gX: ");
    Serial.print(gyro.getX());
    Serial.print(",");
    Serial.print("   gY: ");
    Serial.print(gyro.getY());
    Serial.print(",");
    Serial.print("   gZ: ");
    Serial.print(gyro.getZ());
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

void Mpu::setWriteMode() {
  pinMode(this->ad0_, OUTPUT);
  digitalWrite(this->ad0_, HIGH);
}

Finger::Value Mpu::getFinger() {
  return this->finger_;
}

Mpu::~Mpu() {}
