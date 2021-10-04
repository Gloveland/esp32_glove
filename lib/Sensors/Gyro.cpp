#include <Gyro.h>

extern const bool KDebug;

Gyro::Gyro(const float x_value, const float y_value, const float z_value)
    : x(x_value), y(y_value), z(z_value) {}

float Gyro::getX() const { return this->x; }

float Gyro::getY() const { return this->y; }

float Gyro::getZ() const { return this->z; }

void Gyro::log() const {
  if (!KDebug) {
    return;
  }
  Serial.print("   gX: ");
  Serial.print(this->getX());
  Serial.print(",");
  Serial.print("   gY: ");
  Serial.print(this->getY());
  Serial.print(",");
  Serial.print("   gZ: ");
  Serial.print(this->getZ());
  Serial.print(",");
  Serial.print("  degrees/seg    ");
}

Gyro::~Gyro() = default;