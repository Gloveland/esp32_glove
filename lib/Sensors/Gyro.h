#ifndef GYRO_H_
#define GYRO_H_

/**
 * Measurement of the gyroscope from the IMU sensor.
 */
class Gyro {
  private:
    float x, y, z;

  public:
    Gyro(const float x_value, const float y_value, const float z_value);
    float getX() const;
    float getY() const;
    float getZ() const;
    ~Gyro();
};

#endif // GYRO_H_
