#include <Mpu.h>

const int Mpu::GRAVITY_EARTH = 9.80665F;

Mpu::Mpu(const Finger::Value &finger)
    : finger_(finger),
      ad0_(Finger::getAd0Pin(finger)),
      accelerometer(Accelerometer(Range::_2_G)),
      gyroscope(Gyroscope(GyroRange::_250_DEG)),
      previousTime_(millis()) {}

void Mpu::beginCommunication() { digitalWrite(this->ad0_, LOW); }

void Mpu::endCommunication() { digitalWrite(this->ad0_, HIGH); }

void Mpu::init() {
  this->beginCommunication();
  while (!this->mpu_.begin()) {
    log_e("Failed to find MPU6050 chip");
    delay(10);
  }
 
  this->mpu_.setAccelerometerRange(MPU6050_RANGE_8_G);
  this->mpu_.setGyroRange(MPU6050_RANGE_500_DEG);
  this->mpu_.setFilterBandwidth(MPU6050_BAND_21_HZ);
  this->endCommunication();
}

/**
 * The calibration consist in readings raw measurements values when the sensor
 * is  in horizontal plane. The expected values for acceleration are 0.0
 * in x, 0.0 in y and -9.8 in z. The error is the difference between the
 * expected value and the value read. We sum all the errors and divede by the
 * amount of measurement reads to get the average error in each axis. The same
 * is done with de angle and the gyroscope readings The expected value for the
 * angular velocity is 0.0 for each axis. We calculate the maximum and the
 * minimum value read to get the deviation This way we can  avoid noise in
 * future measurements.
 */
void Mpu::calibrate() {
  log_i("===================== Calibrating %s ====================",
        Finger::getName(this->finger_).c_str());
  this->beginCommunication();

  float sum_acc_x = 0.0, sum_acc_y = 0.0, sum_acc_z = 0.0;
  float sum_gyro_x = 0.0, sum_gyro_y = 0.0, sum_gyro_z = 0.0;

  float min_gyro_x = FLT_MAX, min_gyro_y = FLT_MAX, min_gyro_z = FLT_MAX;
  float max_gyro_x = -FLT_MAX, max_gyro_y = -FLT_MAX, max_gyro_z = -FLT_MAX;

  int times = 100.0;
  sensors_event_t a, g, temp;
  for (int i = 0; i <= times; i++) {
    this->mpu_.getEvent(&a, &g, &temp);

    Acceleration acc = this->accelerometer.fixAcceleration(
        a.acceleration.x, a.acceleration.y, a.acceleration.z);

    log_i("x:%d, y:%d, z:%d", acc.getX(), acc.getY(), acc.getZ());
    Gyro gyro = this->gyroscope.fixGyro(g.gyro.x, g.gyro.x, g.gyro.x);

    sum_acc_x += acc.getX();
    sum_acc_y += acc.getY();
    sum_acc_z += (acc.getZ() + this->GRAVITY_EARTH);

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
}

ImuSensorMeasurement Mpu::read() {
  float currentTime = millis();  // Divide by 1000 to get seconds
  float elapsedTime = (currentTime - this->previousTime_) / 1000;
  this->previousTime_ = currentTime;
  this->beginCommunication();
  sensors_event_t a, g, temp;
  this->mpu_.getEvent(&a, &g, &temp);
  this->endCommunication();
  this->log();
  Acceleration acc = this->accelerometer.fixAcceleration(
      a.acceleration.x, a.acceleration.y, a.acceleration.z);
  Gyro gyro = this->gyroscope.fixGyro(g.gyro.x, g.gyro.y, g.gyro.z);
  ImuSensorMeasurement result = ImuSensorMeasurement(this->finger_, acc, gyro);
  return result;
}

void Mpu::log() { log_d("%s", Finger::getName(this->finger_).c_str()); }

void Mpu::setWriteMode() {
  pinMode(this->ad0_, OUTPUT);
  digitalWrite(this->ad0_, HIGH);
}

Finger::Value Mpu::getFinger() { return this->finger_; }

Mpu::~Mpu() {}
