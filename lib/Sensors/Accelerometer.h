#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

/**
 * Measurement of the accelerometer from the IMU sensor.
 */
struct Acceleration {
  float x, y, z;
  Acceleration() : x(0.0), y(0.0), z(0.0) {}
};

#endif // ACCELEROMETER_H_
