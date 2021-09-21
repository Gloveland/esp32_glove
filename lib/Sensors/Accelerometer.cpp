#include <Accelerometer.h>

extern const bool KDebug;

Accelerometer::Accelerometer(const Range range): range(range), acc_error(Acceleration(0,0,0)){}

Range Accelerometer::getRange(){
    return this->range;
}

void Accelerometer::setError(int times, float sum_acc_x, float sum_acc_y, float sum_acc_z){
  float acc_error_x = sum_acc_x / times;
  float acc_error_y = sum_acc_y / times;
  float acc_error_z = sum_acc_z / times;
  this->acc_error = Acceleration(acc_error_x, acc_error_y, acc_error_z);
  this->logError();
  
}

Acceleration Accelerometer::readAcc(const int16_t raw_acc_x, const int16_t raw_acc_y, const int16_t raw_acc_z) {
  float accScale = this->getScale(this->range);
  float x_value = ((float) raw_acc_x / accScale) * GRAVITY_EARTH - this->acc_error.getX();
  float y_value = ((float) raw_acc_y / accScale) * GRAVITY_EARTH - this->acc_error.getY();
  float z_value = ((float) raw_acc_z / accScale) * GRAVITY_EARTH - this->acc_error.getZ();
  Acceleration current_acceleration = Acceleration(x_value, y_value, z_value);
  current_acceleration.log();
  
  return current_acceleration;
}

float Accelerometer::getScale(const Range range) {
  switch (range) {
    case Range::_16_G:return (float) Scale::_16;
    case Range::_8_G:return (float) Scale::_8;
    case Range::_4_G:return (float) Scale::_4;
    default:return (float) Scale::_2;
  }
}

void Accelerometer::logError() {
  if(!KDebug){
    return;
  }
  Serial.println("");
  Serial.print("AccErrorX: ");
  Serial.println(this->acc_error.getX());
  Serial.print("AccErrorY: ");
  Serial.println(this->acc_error.getY());
  Serial.print("AccErrorZ: ");
  Serial.println(this->acc_error.getZ());
}



Accelerometer::~Accelerometer() = default;