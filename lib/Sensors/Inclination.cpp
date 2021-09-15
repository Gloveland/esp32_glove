#include "Inclination.h"

Inclination::Inclination(const float roll_value, const float pitch_value, const float yaw_value):
  roll(roll_value), 
  pitch(pitch_value),
  yaw(yaw_value){
}

float Inclination::getRoll() const { return this->roll; }
float Inclination::getPitch() const { return this->pitch; }
float Inclination::getYaw() const { return this->yaw; }

void Inclination::log(){
  Serial.print("  roll:");
  Serial.print(this->getRoll());
  Serial.print("  pitch:");
  Serial.print(this->getPitch());
  Serial.print("  yaw:");
  Serial.print(this->getYaw());
}

Inclination::~Inclination() = default;