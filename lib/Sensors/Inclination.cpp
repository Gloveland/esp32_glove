#include "Inclination.h"

float Inclination::CalculateAccAngleX(const Acceleration acc) {
  return (atan(acc.y / sqrt(pow(acc.x, 2) + pow(acc.z, 2))) * 180 / PI);
}

float Inclination::CalculateAccAngleY(const Acceleration acc) {
  return (atan(-1 * acc.x / sqrt(pow(acc.y, 2) + pow(acc.z, 2))) * 180 / PI);
}

Inclination Inclination::computeInclination(const Acceleration acc,
                                            const Gyro gyro,
                                            const float elapsedTime) {
  this->inclination_angle.x =
      this->inclination_angle.x + gyro.x * elapsedTime;
  this->inclination_angle.y =
      this->inclination_angle.y + gyro.y * elapsedTime;
  this->inclination_angle.z =
      this->inclination_angle.z + gyro.z * elapsedTime;

  float inclination_from_acc_x =
      CalculateAccAngleX(acc) - this->inclination_from_acc_error.x;
  float inclination_from_acc_y =
      CalculateAccAngleY(acc) - this->inclination_from_acc_error.y;

  if (abs(inclination_from_acc_x - this->inclination_angle.x) > 5.0) {
    this->inclination_angle.x = inclination_from_acc_x;
  }
  if (abs(inclination_from_acc_y - this->inclination_angle.y) > 5.0) {
    this->inclination_angle.y = inclination_from_acc_y;
  }
  Inclination inclination;
  inclination.roll = this->inclination_angle.x;
  inclination.pitch = this->inclination_angle.y;
  inclination.yaw = this->inclination_angle.z;
  return inclination;
}

void Inclination::setInclinationError(int times,
                                      float sum_angle_from_acc_x,
                                      float sum_angle_from_acc_y) {
  this->inclination_from_acc_error.x = sum_angle_from_acc_x / times;
  this->inclination_from_acc_error.y = sum_angle_from_acc_y / times;
}
