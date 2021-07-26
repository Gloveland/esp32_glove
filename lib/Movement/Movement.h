
#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <Arduino_JSON.h>
#include <WiFi.h>

using namespace std;

struct Acceleration {
  float x, y, z;
};

struct Gyro {
  float x, y, z;
};

struct Inclination {
  float roll;
  float pitch;
  float yaw;
};

struct FingerMov {
  Acceleration acc;
  Gyro gyro;
  Inclination inclination;
};

struct HandMov {
  FingerMov pinky;
  // FingerMov ring;
  // FingerMov middle;
  // FingerMov index;
  FingerMov thump;
};

class Movement {
 public:
  Movement(int event_num, string deviceId, HandMov hand);
  void send(WiFiClient client);
  ~Movement();

 private:
  int eventNum;
  string deviceId;
  HandMov hand;
  JSONVar getFingerJson(FingerMov finger);
  string format(float measurement);
};

#endif  // MOVEMENT_H_
