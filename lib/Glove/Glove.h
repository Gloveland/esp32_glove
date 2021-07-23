#ifndef GLOVE_H
#define GLOVE_H

#define I2C_SCL_PINKY 27 /* gris */
#define I2C_SDA_PINKY 26 /* violeta */

#define I2C_SCL_RING 33 /* azul */
#define I2C_SDA_RING 32 /* verde */

#define I2C_SCL_MIDDLE 25 /* amarillo */
#define I2C_SDA_MIDDLE 23 /* naranja */

#define I2C_SCL_INDEX 19 /* azul */
#define I2C_SDA_INDEX 18 /* verde */

#define I2C_SCL_THUMB 22 /* amarillo */
#define I2C_SDA_THUMB 21 /* naranja */

#include <Movement.h>
#include <Mpu.h>

class Glove {
 public:
  Glove();
  void init();
  Movement readMovement(const int eventNum);
  ~Glove();

 private:
  string chipId;
  // Mpu pinkySensor;
  // Mpu ringSensor;
  // Mpu middleSensor;
  // Mpu indexSensor;
  Mpu thumpSensor;
  string getChipId();
};

#endif  // GLOVE_H