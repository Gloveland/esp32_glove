#include <Acceleration.h>
#define PI 3.1415926535897932384626433832795

Acceleration::Acceleration(float x_value, float y_value, float z_value)
    : x(x_value), y(y_value), z(z_value) {}

float Acceleration::getX() const { return this->x; }

float Acceleration::getY() const { return this->y; }

float Acceleration::getZ() const { return this->z; }

float Acceleration::calculateAngleX() const {
  double divisor = sqrt(pow(this->x, 2) + pow(this->z, 2));
  if (divisor == 0) {  // probably sensor reading error
    return 0;
  }
  return (atan(this->y / divisor) * 180 / PI);
}

float Acceleration::calculateAngleY() const {
  double divisor = sqrt(pow(this->y, 2) + pow(this->z, 2));
  if (divisor == 0) {  // probably sensor reading error
    return 0;
  }
  return (atan(-1 * this->x / divisor) * 180 / PI);
}

void Acceleration::log() {
  log_d("aX: %.2f  aY: %.2f  aZ: %.2f  m/(seg)^2", this->getX(), this->getY(),
        this->getZ());
}

Acceleration::~Acceleration() = default;