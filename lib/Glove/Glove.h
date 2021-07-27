#ifndef GLOVE_H
#define GLOVE_H


#define I2C_SCL 22 
#define I2C_SDA 21 

#define PINKY 17 
#define RING  27
#define MIDDLE 26 
#define INDEX 32 
#define THUMB 33 

#include <Movement.h>
#include <Mpu.h>

class Glove {
 public:
  Glove();
  void init();
  Movement readMovement(const int eventNum);
  ~Glove();

 private:
  std::string  chipId;
  Mpu pinkySensor;
  // Mpu ringSensor;
  // Mpu middleSensor;
  // Mpu indexSensor;
  Mpu thumbSensor;
  std::string  getChipId();
};

#endif  // GLOVE_H
