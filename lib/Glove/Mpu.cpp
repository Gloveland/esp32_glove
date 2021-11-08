#include <Mpu.h>

#include "MPU6050.h"

const int Mpu::HEX_ADDRESS = 16;
const int Mpu::OK = 0;
const int Mpu::GRAVITY_EARTH = 9.80665F;
const int Mpu::GENERAL_CONFIG = 0x1A;  ///< General configuration register
const int Mpu::PWR_MGMT_1 = 0x6B;
const int Mpu::RESET = 0x00;
const int Mpu::ACC_CONFIG_REGISTER = 0x1C;
const int Mpu::GYRO_CONFIG_REGISTER = 0x1C;
const int Mpu::ACCEL_XOUT_H = 0x3B;
const int Mpu::GYRO_XOUT_H = 0x43;
const int Mpu::ALL_REGISTERS = 14;
const int Mpu::BITS_IN_BYTE = 8;
const int Mpu::TEMP_DIS_PLL = 0x09;  // temperature disabled: 0x08 and
// PLL with X axiS gyroscope reference: 0x01 => 0X09

Mpu::Mpu(const Finger::Value &finger)
    : finger_(finger),
      ad0_(Finger::getAd0Pin(finger)),
      accelerometer(Accelerometer(Range::_2_G)),
      gyroscope(Gyroscope(GyroRange::_250_DEG)),
      inclination_calculator(InclinationCalculator()),
      previousTime_(millis()) {}

void Mpu::beginCommunication() { digitalWrite(this->ad0_, LOW); }

void Mpu::endCommunication() { digitalWrite(this->ad0_, HIGH); }

void Mpu::init() {
  this->beginCommunication();

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(Mpu::PWR_MGMT_1);
  Wire.write(Mpu::RESET);
  Wire.endTransmission(true);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(Mpu::ACC_CONFIG_REGISTER);
  Wire.write(this->accelerometer
                 .getRange());  // ex 00010000 = 0x10  (+/- 8g full scale range)
  Wire.endTransmission(true);
  delay(20);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(Mpu::GYRO_CONFIG_REGISTER);
  Wire.write(
      this->gyroscope.getGyroRange());  // ex: 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(Mpu::GENERAL_CONFIG);
  Wire.write(mpuBand::_21_HZ);  // ex: 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);

  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(Mpu::PWR_MGMT_1);
  Wire.write(Mpu::TEMP_DIS_PLL);
  Wire.endTransmission(true);
  delay(20);

  MPU6050 mpu6050;
  uint8_t speed = 13;
  mpu6050.setMasterClockSpeed(speed);

  this->endCommunication();
}

/**
 * The calibration consist in readings raw measurements values when the sensor
 * is  in horizontal plane. The expected values for acceleration are 0.0
 * in x, 0.0 in y and 9.8 in z. The error is the difference between the expected
 * value and the value read. We sum all the errors and divede by the amount of
 * measurement reads to get the average error in each axis. The same is done
 * with de angle and the gyroscope readings The expected value for the angular
 * velocity is 0.0 for each axis. We calculate the maximum and the minimum value
 * read to get the deviation This way we can  avoid noise in future
 * measurements.
 */
void Mpu::calibrate() {
  log_i("===================== Calibrating %s ====================",
        Finger::getName(this->finger_).c_str());
  this->beginCommunication();

  float sum_acc_x = 0.0, sum_acc_y = 0.0, sum_acc_z = 0.0;
  float acc_angle_x = 0.0, acc_angle_y = 0.0;
  float sum_angle_from_acc_x = 0.0, sum_angle_from_acc_y = 0.0;
  float sum_gyro_x = 0.0, sum_gyro_y = 0.0, sum_gyro_z = 0.0;

  float min_gyro_x = FLT_MAX, min_gyro_y = FLT_MAX, min_gyro_z = FLT_MAX;
  float max_gyro_x = -FLT_MAX, max_gyro_y = -FLT_MAX, max_gyro_z = -FLT_MAX;

  int times = 100.0;
  for (int i = 0; i <= times; i++) {
    RawMeasurement raw = this->readAllRaw();

    Acceleration acc =
        this->accelerometer.readAcc(raw.acc_x, raw.acc_y, raw.acc_z);
    Gyro gyro = this->gyroscope.readGyro(raw.gyro_x, raw.gyro_y, raw.gyro_z);
    acc_angle_x = acc.calculateAngleX();
    acc_angle_y = acc.calculateAngleY();

    sum_acc_x += acc.getX();
    sum_acc_y += acc.getY();
    sum_acc_z += (acc.getZ() - this->GRAVITY_EARTH);
    sum_angle_from_acc_x += acc_angle_x;
    sum_angle_from_acc_y += acc_angle_y;

    if (gyro.getX() < min_gyro_x) {
      min_gyro_x = gyro.getX();
    }
    if (gyro.getY() < min_gyro_y) {
      min_gyro_y = gyro.getY();
    }
    if (gyro.getZ() < min_gyro_z) {
      min_gyro_z = gyro.getZ();
    }
    if (gyro.getX() > max_gyro_x) {
      max_gyro_x = gyro.getX();
    }
    if (gyro.getY() > max_gyro_y) {
      max_gyro_y = gyro.getY();
    }
    if (gyro.getZ() > max_gyro_z) {
      max_gyro_z = gyro.getZ();
    }
    sum_gyro_x += gyro.getX();
    sum_gyro_y += gyro.getY();
    sum_gyro_z += gyro.getZ();
    delay(20);
  }
  this->endCommunication();
  this->accelerometer.setError(times, sum_acc_x, sum_acc_y, sum_acc_z);
  this->gyroscope.setGyroError(times, sum_gyro_x, sum_gyro_y, sum_gyro_z);
  this->gyroscope.setDeviation(times, max_gyro_x, max_gyro_y, max_gyro_z,
                               min_gyro_x, min_gyro_y, min_gyro_z);
  this->inclination_calculator.setError(times, sum_angle_from_acc_x,
                                        sum_angle_from_acc_y);
}

ImuSensorMeasurement Mpu::read() {
  float currentTime = millis();  // Divide by 1000 to get seconds
  float elapsedTime = (currentTime - this->previousTime_) / 1000;
  this->previousTime_ = currentTime;
  RawMeasurement raw = this->readAllRaw();
  this->log();
  Acceleration acc =
      this->accelerometer.readAcc(raw.acc_x, raw.acc_y, raw.acc_z);
  Gyro gyro = this->gyroscope.readGyro(raw.gyro_x, raw.gyro_y, raw.gyro_z);
  Inclination inclination =
      this->inclination_calculator.calculateInclination(acc, gyro, elapsedTime);
  ImuSensorMeasurement result =
      ImuSensorMeasurement(this->finger_, acc, gyro, inclination);
  return result;
}

/**
 * We read 14 registers from mpu (each value is stored in 2 registers)
 * the registers order is: | ax | ay | az | temp | gx | gy | gz |
 */
RawMeasurement Mpu::readAllRaw() {
  this->beginCommunication();
  Wire.beginTransmission(mpuAddress::_ON);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(mpuAddress::_ON, ALL_REGISTERS, true);

  RawMeasurement rawMeasurement;
  rawMeasurement.acc_x = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.acc_y = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.acc_z = (Wire.read() << BITS_IN_BYTE | Wire.read());

  rawMeasurement.temp = (Wire.read() << BITS_IN_BYTE | Wire.read());

  rawMeasurement.gyro_x = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.gyro_y = (Wire.read() << BITS_IN_BYTE | Wire.read());
  rawMeasurement.gyro_z = (Wire.read() << BITS_IN_BYTE | Wire.read());
  this->endCommunication();
  return rawMeasurement;
}

void Mpu::log() { log_d("%s", Finger::getName(this->finger_).c_str()); }

void Mpu::setWriteMode() {
  pinMode(this->ad0_, OUTPUT);
  digitalWrite(this->ad0_, HIGH);
}

Finger::Value Mpu::getFinger() { return this->finger_; }

Mpu::~Mpu() {}
