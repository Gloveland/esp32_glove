
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

struct SensorMeasurement {
  Acceleration acc;
  Gyro gyro;
  Inclination inclination;
};

struct HandMov {
  SensorMeasurement pinky;
  // SensorMeasurement ring;
  // SensorMeasurement middle;
  // SensorMeasurement index;
  SensorMeasurement thumb;
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
  JSONVar measurementToJSON(SensorMeasurement sensorMeasurement);
  string format(float measurement);
};

#endif  // MOVEMENT_H_
