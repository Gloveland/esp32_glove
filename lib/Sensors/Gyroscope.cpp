#include <Gyroscope.h>


Gyroscope::Gyroscope(const GyroRange gyro_range)
    : gyro_range_(gyro_range),
      gyro_error_(Gyro(0, 0, 0)),
      previous_gyro_(Gyro(0, 0, 0)),
      deviation_(Gyro(0, 0, 0)) {}

void Gyroscope::setGyroError(int times, float sum_gyro_x, float sum_gyro_y,
                             float sum_gyro_z) {
  float x = sum_gyro_x / times;
  float y = sum_gyro_y / times;
  float z = sum_gyro_z / times;
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
Gyro Gyroscope::readGyro(const float gyro_x, const float gyro_y,
                         const float gyro_z) {

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
  this->previous_gyro_.log();

  return Gyro(x_value, y_value, z_value);
}

void Gyroscope::logGyroError() {
  log_i("GyroErrorX:: %.2f  GyroErrorY: %.2f  GyroErrorXZ: %.2f",
        this->gyro_error_.getX(),
        this->gyro_error_.getY(),
        this->gyro_error_.getZ());
}

void Gyroscope::logDeviation() {
  log_i("DeviationX:: %.2f  DeviationY: %.2f  DeviationZ: %.2f",
        this->deviation_.getX(),
        this->deviation_.getY(),
        this->deviation_.getZ());
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
