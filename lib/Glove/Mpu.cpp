#include <Mpu.h>

Mpu::Mpu(const Finger::Value &finger, bool debug):
    finger_(finger),
    ad0_(Finger::getAd0Pin(finger)),
    debug(debug),
    accelerometer(Accelerometer(Range::_2_G, debug)),
    gyroscope(Gyroscope(GyroRange::_250_DEG, debug)),
    inclination_calculator(InclinationCalculator(debug)),
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
  if (error != this->OK) {
    Serial.print("Error checking address: ");
    Serial.print(" 0x");
    if (address < this->HEX_ADDRESS) {
      Serial.print("0");
    }
    Serial.println(address, HEX);
    if (error == this->DATA_BUFFER_ERROR) {
      Serial.print(" Data too long to fit in transmit buffer. ");
      Serial.println(" Is i2c bus initialize?");
    }
    if (error == this->UNKNOWN_ERROR) {
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
  Wire.write(this->gyroscope.getGyroRange());  // ex: 00010000 (1000deg/s full scale)
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
    Gyro gyro = this->gyroscope.readGyro(raw.gyroX, raw.gyroY, raw.gyroZ);
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
  this->gyroscope.setGyroError(times, sumGyroX, sumGyroY, sumGyroZ);
  this->gyroscope.setDeviation(times, maxGyroX, maxGyroY, maxGyroZ, minGyroX, minGyroY, minGyroZ);
  this->inclination_calculator.setError(times, sumAngleFromAccX, sumAngleFromAccY);

                     
}


ImuSensorMeasurement Mpu::read() {
  this->beginCommunication();
  float currentTime = millis();  // Divide by 1000 to get seconds
  float elapsedTime = (currentTime - this->previousTime_) / 1000;
  this->previousTime_ = currentTime;
  RawMeasurement raw = this->readAllRaw();
  this->endCommunication();

  if (this->debug) {
    Serial.print(Finger::getName(this->finger_).c_str());
  }
  Acceleration acc = this->accelerometer.readAcc(raw.accX, raw.accY, raw.accZ);
  Gyro gyro = this->gyroscope.readGyro(raw.gyroX, raw.gyroY, raw.gyroZ);
  Inclination inclination = this->inclination_calculator.calculateInclination(acc, gyro, elapsedTime);
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



void Mpu::log() {
  Serial.print(Finger::getName(this->finger_).c_str());
}

float Mpu::readTemperature(const int16_t rawTemp) {
  float temperature = (rawTemp / 340.0) + 36.53;
  return temperature;
}

void Mpu::setWriteMode() {
  pinMode(this->ad0_, OUTPUT);
  digitalWrite(this->ad0_, HIGH);
}

Finger::Value Mpu::getFinger() {
  return this->finger_;
}

Mpu::~Mpu() {}
