#include <Gyro.h>

Gyro::Gyro(const float x_value, const float y_value, const float z_value):
    x(x_value),
    y(y_value),
    z(z_value){}


float Gyro::getX() const{
    return this->x;
}

float Gyro::getY() const{
    return this->y;
}

float Gyro::getZ() const{
    return this->z;
}

Gyro::~Gyro() = default;