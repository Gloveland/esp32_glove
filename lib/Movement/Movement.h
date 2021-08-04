
#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <Arduino_JSON.h>
#include <WiFi.h>

struct Acceleration {
  float x, y, z;
  Acceleration() : x(0.0), y(0.0), z(0.0) {}
};

struct Gyro {
  float x, y, z;
  Gyro() : x(0.0), y(0.0), z(0.0) {}
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
  float temperature;
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
  Movement(int event_num, std::string  deviceId, HandMov hand);
  void send(WiFiClient client);
  ~Movement();

 private:
  int eventNum;
  std::string deviceId;
  HandMov hand;
  JSONVar measurementToJSON(SensorMeasurement sensorMeasurement);
  std::string format(float measurement);
};

#endif  // MOVEMENT_H_
