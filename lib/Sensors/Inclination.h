#ifndef INCLINATION_H_
#define INCLINATION_H_

#include <Arduino.h>
#include <math.h>
#include "Accelerometer.h"
#include "Gyroscope.h"

/**
 * Measurement of the inclination in degrees.
 * We get the inclination angle from the gyroscope readings along with the
 * values provided by the accelerometer.
 *
 * <p>The complete rotation angle is the result of the accumulation of all the
 * rotation angles over time. The rotation angle is computed as the result of
 * the angular velocity over the elapsed time between two successive
 * measurements.
 *
 * <p>The constant addition of these measurements could lead to accumulation of
 * noise resulting in infinite values. In order to avoid this problem we
 * adjust the inclination on the x-axis and y-axis using the gravity
 * acceleration change. In case the rotation angle calculated from gyroscope
 * exceeds the rotation angle calculated from the acceleration by 5 units we
 * dismiss the accumulated angle and use the acceleration calculated angle
 * instead. This trick can't be applied to the z-axis because if we rotate the
 * in the x plane the gravity force direction doesn't change.
 */
class Inclination {
 public:
  float roll;
  float pitch;
  float yaw;

  Inclination() : roll(0.0), pitch(0.0), yaw(0.0) {}

  Inclination computeInclination(Acceleration acc,
                                 Gyro gyro,
                                 float elapsedTime);

  void setInclinationError(int times,
                           float sum_angle_from_acc_x,
                           float sum_angle_from_acc_y);
 private:
  Gyro inclination_angle{};
  Acceleration inclination_from_acc_error{};

  static float CalculateAccAngleX(Acceleration acc);

  static float CalculateAccAngleY(Acceleration acc);
};

#endif // INCLINATION_H_
