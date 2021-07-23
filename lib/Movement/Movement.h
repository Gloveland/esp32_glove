//
// Created by Jazmin Ferreiro on 22/07/2021.
//

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#define FORMATTED_JSON_MEASUREMENT                                            \
  "{\"device_id\":\"%s\", \"event_num\":%d, \"hand\": { \"thump\": { "        \
  "\"acc\": { \"x\":%.3f, \"y\":%.3f, \"z\":%.3f }, \"gyro\": { \"x\":%.3f, " \
  "\"y\":%.3f, \"z\":%.3f }, \"inclination\": { \"roll\":%.3f, "              \
  "\"pitch\":%.3f, \"yaw\":%.3f } } } }\n"


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
  Finger thump;
  //Finger index;
  //Finger middle;
  //Finger ring;
  //Finger pinky;
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
};

#endif //MOVEMENT_H_

