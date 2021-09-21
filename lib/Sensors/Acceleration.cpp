#include <Acceleration.h>
#define PI 3.1415926535897932384626433832795

extern const bool KDebug;


Acceleration::Acceleration(float x_value, float y_value, float z_value)
    : x(x_value), y(y_value), z(z_value) {}

float Acceleration::getX() const { return this->x; }

float Acceleration::getY() const { return this->y; }

float Acceleration::getZ() const { return this->z; }

float Acceleration::calculateAngleX() const {
  return (atan(this->y / sqrt(pow(this->x, 2) + pow(this->z, 2))) * 180 / PI);
}

float Acceleration::calculateAngleY() const {
  return (atan(-1 * this->x / sqrt(pow(this->y, 2) + pow(this->z, 2))) * 180 /PI);
}

void Acceleration::log() {
  if(!KDebug){
    return;
  }
  Serial.print("   aX: ");
  Serial.print(this->getX());
  Serial.print("   aY: ");
  Serial.print(this->getY());
  Serial.print("   aZ: ");
  Serial.print(this->getZ());
  Serial.print(",");
  Serial.print("   m/(seg)^2    ");
}

Acceleration::~Acceleration() = default;