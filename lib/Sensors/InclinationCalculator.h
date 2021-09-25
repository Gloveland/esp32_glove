#ifndef INCLINATION_CALCULATOR_H_
#define INCLINATION_CALCULATOR_H_

#include <Arduino.h>
#include <math.h>

#include "Acceleration.h"
#include "Gyro.h"
#include "Inclination.h"

class InclinationCalculator {
 public:
  InclinationCalculator();
  Inclination calculateInclination(const Acceleration current_acc,
                                   const Gyro current_gyro,
                                   const float elapsedTime);
  void setError(int times, float sum_angle_from_acc_x,
                float sum_angle_from_acc_y);
  void log(const float inclination_from_acc_x,
           const float inclination_from_acc_y);
  ~InclinationCalculator();

 private:
  Acceleration inclination_angle_from_acc_error;
  Inclination previous_inclination;
};

#endif  // INCLINATION_CALCULATOR_H_