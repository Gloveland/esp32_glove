#ifndef GLOVE_H
#define GLOVE_H


#include <Movement.h>
#include <Mpu.h>

enum mpuAddressPin { 
  PINKY = 17, 
  RING = 27, 
  MIDDLE = 26, 
  INDEX = 32 , 
  THUMB = 33
};

class Glove {
 public:
  Glove();
  void init();
  Movement readMovement(const int eventNum);
  ~Glove();

 private:
  const int I2C_SCL = 22;
  const int I2C_SDA = 21;
  std::string  chipId;
  Mpu pinkySensor;
  // Mpu ringSensor;
  // Mpu middleSensor;
  // Mpu indexSensor;
  Mpu thumbSensor;
  std::string  getChipId();
};

#endif  // GLOVE_H
