#ifndef INCLINATION_H_
#define INCLINATION_H_

#include <Arduino.h>

class Inclination {
 public:
  Inclination(const float roll_value, const float pitch_value, const float yaw_value);
  float getRoll() const;
  float getPitch() const;
  float getYaw() const;
  void log();
  ~Inclination();

 private:
  float roll;
  float pitch;
  float yaw;
};

#endif // INCLINATION_H_
