#ifndef FINGER_H_
#define FINGER_H_

#include <map>

/**
 * Struct containing the information associated to the fingers of the hand in
 * which the Imu sensors will be located, such as the name of each finger as
 * well as the AD0 pin that will be used in order to manage the I2C
 * communication between the nodemcu esp32 and the sensors.
 */
class Finger {
 public:
  enum Value {
    kPinky,
    kRing,
    kMiddle,
    kIndex,
    kThumb,
  };

  /**
   * Gets the name of the finger passed as parameter.
   */
  static std::string getName(Value finger) {
    switch (finger) {
      case kPinky:return "pinky";
      case kRing:return "ring";
      case kMiddle:return "middle";
      case kIndex:return "index";
      case kThumb:return "thumb";
    }
  }

  /**
   * Returns the AD0 pin finger used for communication with the associated
   * IMU using the I2C bus between the nodemcu esp32 and the sensor.
   */
  static u_int getAd0Pin(Value finger) {
    switch (finger) {
      case kPinky:return 33;
      case kRing:return 27;
      case kMiddle:return 26;
      case kIndex:return 32;
      case kThumb:return 17;
    }
  }

 private:
  Finger() = default;
};

#endif // FINGER_H_
