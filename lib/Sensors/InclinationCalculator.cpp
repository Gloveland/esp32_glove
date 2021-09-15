#include "InclinationCalculator.h"

InclinationCalculator::InclinationCalculator(bool debug)
    : debug(debug),
      inclination_angle_from_acc_error(Acceleration(0, 0, 0)),
      previous_inclination(Inclination(0, 0, 0)) {}

void InclinationCalculator::setError(int times, float sum_angle_from_acc_x,
                                     float sum_angle_from_acc_y) {
  float inclination_angle_error_x = sum_angle_from_acc_x / times;
  float inclination_angle_error_y = sum_angle_from_acc_y / times;
  this->inclination_angle_from_acc_error =
      Acceleration(inclination_angle_error_x, inclination_angle_error_y, 0);
}

void InclinationCalculator::log(const float inclination_from_acc_x,
                                const float inclination_from_acc_y) {
  Serial.print("   accAngleX: ");
  Serial.print(inclination_from_acc_x);
  Serial.print("   accAngleY: ");
  Serial.print(inclination_from_acc_y);
}

Inclination InclinationCalculator::calculateInclination(
    const Acceleration current_acc, const Gyro current_gyro,
    const float elapsedTime) {
  float current_inclination_angle_x =
      this->previous_inclination.getRoll() + current_gyro.getX() * elapsedTime;
  float current_inclination_angle_y =
      this->previous_inclination.getPitch() + current_gyro.getY() * elapsedTime;
  float current_inclination_angle_z =
      this->previous_inclination.getYaw() + current_gyro.getZ() * elapsedTime;

  float inclination_from_acc_x = current_acc.calculateAngleX() -
                                 this->inclination_angle_from_acc_error.getX();
  float inclination_from_acc_y = current_acc.calculateAngleY() -
                                 this->inclination_angle_from_acc_error.getY();

  if (this->debug) {
    this->log(inclination_from_acc_x, inclination_from_acc_y);
  }
  if (abs(inclination_from_acc_x - current_inclination_angle_x) > 5.0) {
    current_inclination_angle_x = inclination_from_acc_x;
  }
  if (abs(inclination_from_acc_y - current_inclination_angle_y) > 5.0) {
    current_inclination_angle_y = inclination_from_acc_y;
  }
  this->previous_inclination =
      Inclination(current_inclination_angle_x, current_inclination_angle_y,
                  current_inclination_angle_z);

  if (debug) {
    this->previous_inclination.log();
  }
  return this->previous_inclination;
}

InclinationCalculator::~InclinationCalculator() = default;