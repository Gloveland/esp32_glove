#include <Gyroscope.h>

Gyroscope::Gyroscope(const GyroRange gyro_range, const bool debug)
    : debug(debug),
      gyro_range_(gyro_range),
      gyro_error_(Gyro(0, 0, 0)),
      previous_gyro_(Gyro(0, 0, 0)),
      deviation_(Gyro(0, 0, 0)) {}

void Gyroscope::setGyroError(int times, float sum_gyro_x, float sum_gyro_y,
                             float sum_gyro_z) {

  float x = sum_gyro_x / times;
  float y = sum_gyro_y / times;
  float z = sum_gyro_z / times;
  Serial.println(" set gyro error:");
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
  this->gyro_error_ = Gyro(x, y, z);
  this->logGyroError();
}

void Gyroscope::setDeviation(int times, float maxX, float maxY, float maxZ,
                             float minX, float minY, float minZ) {
  float x = (maxX - minX) / 2.0;
  float y = (maxY - minY) / 2.0;
  float z = (maxZ - minZ) / 2.0;
  this->deviation_ = Gyro(x, y, z);
  this->logDeviation();
}

GyroRange Gyroscope::getGyroRange() { return this->gyro_range_; }

/**
 * Get angular velocity of rotation from raw gyro readings.
 * If the difference between the measurement and the previous mesurement
 * is lower than the maximum deviation we dismiss the measurement because is
 * considered noise
 */
Gyro Gyroscope::readGyro(const int16_t rawGyro_x, const int16_t rawGyro_y,
                         const int16_t rawGyro_z) {
  float gyro_scale = this->getGyroScale(this->gyro_range_);
  float gyro_x = ((float)rawGyro_x / gyro_scale);
  float gyro_y = ((float)rawGyro_y / gyro_scale);
  float gyro_z = ((float)rawGyro_z / gyro_scale);

  float x_value, y_value, z_value = 0.0;
  if (abs(this->previous_gyro_.getX() - gyro_x) > this->deviation_.getX()) {
    x_value = gyro_x - this->gyro_error_.getX();
  }

  if (abs(this->previous_gyro_.getY() - gyro_y) > this->deviation_.getY()) {
    y_value = gyro_y - this->gyro_error_.getY();
  }

  if (abs(this->previous_gyro_.getZ() - gyro_z) > this->deviation_.getZ()) {
    z_value = gyro_z - this->gyro_error_.getZ();
  }
  this->previous_gyro_ = Gyro(gyro_x, gyro_y, gyro_z);
  if (this->debug) {
    this->previous_gyro_.log();
  }

  return Gyro(x_value, y_value, z_value);
}

void Gyroscope::logGyroError() {
  Serial.println("");
  Serial.print("GyroErrorX: ");
  Serial.println(this->gyro_error_.getX());
  Serial.print("GyroErrorY: ");
  Serial.println(this->gyro_error_.getY());
  Serial.print("GyroErrorZ: ");
  Serial.println(this->gyro_error_.getZ());
}

void Gyroscope::logDeviation() {
  Serial.print("DeviationX: ");
  Serial.println(this->deviation_.getX());
  Serial.print("DeviationY: ");
  Serial.println(this->deviation_.getY());
  Serial.print("DeviationZ: ");
  Serial.println(this->deviation_.getZ());
}

float Gyroscope::getGyroScale(const GyroRange gyro_range) {
  switch (gyro_range) {
    case GyroRange::_2000_DEG:
      return (float)GyroScaleX10::_2000 / GYRO_SCALE_DIVISOR;
    case GyroRange::_1000_DEG:
      return (float)GyroScaleX10::_1000 / GYRO_SCALE_DIVISOR;
    case GyroRange::_500_DEG:
      return (float)GyroScaleX10::_500 / GYRO_SCALE_DIVISOR;
    default:
      return (float)GyroScaleX10::_250 / GYRO_SCALE_DIVISOR;
  }
}

Gyroscope::~Gyroscope() = default;
