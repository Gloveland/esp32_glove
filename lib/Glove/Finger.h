#ifndef FINGER_H_
#define FINGER_H_

#include <map>

class Finger {
 public:
  enum Value {
    kPinky,
    kRing,
    kMiddle,
    kIndex,
    kThumb,
  };

  static std::string getName(Value value) {
    switch (value) {
      case kPinky:return "pinky";
      case kRing:return "ring";
      case kMiddle:return "middle";
      case kIndex:return "index";
      case kThumb:return "thumb";
    }
  }

  static u_int getAd0Pin(Value value) {
    switch (value) {
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
