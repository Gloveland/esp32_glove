#include <Accelerometer.h>

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

Acceleration Accelerometer::fixAcceleration(const float acc_x, const float acc_y, const float acc_z) {
  float accScale = this->getScale(this->range);
  float x_value = acc_x - this->acc_error.getX();
  float y_value = acc_y - this->acc_error.getY();
  float z_value = acc_z - this->acc_error.getZ();
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
  log_i("AccErrorX: %3.f  AccErrorY: %3.f  AccErrorZ: %3.f",this->acc_error.getX(), this->acc_error.getY(), this->acc_error.getZ());
}



Accelerometer::~Accelerometer() = default;