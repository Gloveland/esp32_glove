#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

/**
 * Measurement of the gyroscope from the IMU sensor.
 */
struct Gyro {
  float x, y, z;
  Gyro() : x(0.0), y(0.0), z(0.0) {}
};

#endif // GYROSCOPE_H_