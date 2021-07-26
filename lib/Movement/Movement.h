
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

struct Finger {
  Acceleration acc;
  Gyro gyro;
  Inclination inclination;
};

struct Hand {
  Finger pinky;
  // Finger ring;
  // Finger middle;
  // Finger index;
  Finger thump;
};

class Movement {
 public:
  Movement(int event_num, string deviceId, Hand hand);
  void send(WiFiClient client);
  ~Movement();

 private:
  int eventNum;
  string deviceId;
  Hand hand;
  JSONVar getFingerJson(Finger finger);
  string format(float measurement);
};

#endif  // MOVEMENT_H_
