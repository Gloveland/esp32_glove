#ifndef ACCELERATION_H_
#define ACCELERATION_H_

#include <Arduino.h>
#include <math.h>
/**
 * Measurement of the accelerometer from the IMU sensor.
 */
class Acceleration {
  private:
    float x;
    float y;
    float z;

  public:
    Acceleration(float x_value, float y_value, float z_value);
    void log();
    float getX() const;
    float getY() const;
    float getZ() const;
    float calculateAngleX() const;
    float calculateAngleY() const;
     ~Acceleration();

};

#endif // ACCELERATION_H_
