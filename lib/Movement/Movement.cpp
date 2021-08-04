#include "Movement.h"

Movement::Movement(int num, std::string  id, HandMov hand)
    : eventNum(num), deviceId(id), hand(hand) {}

void Movement::send(WiFiClient client) {
  JSONVar jsonHand;
  // jsonHand["index"] = this->getFingerJson(this->hand.index);
  // jsonHand["middle"] = this->getFingerJson(this->hand.middle);
  // jsonHand["ring"] = this->getFingerJson(this->hand.ring);
  // jsonHand["pinky"] = this->getFingerJson(this->hand.pinky);
  JSONVar thumb = this->measurementToJSON(this->hand.thumb);

  jsonHand["thumb"] = thumb;
  JSONVar jsonMovement;
  jsonMovement["device_id"] = this->deviceId.c_str();
  jsonMovement["event_num"] = this->eventNum;
  jsonMovement["hand"] = jsonHand;
  Serial.print("  sending value via wifi: ");
  String jsonString = JSON.stringify(jsonMovement);
  Serial.println(jsonString);
  int bytes_sent = client.write(jsonString.c_str(), jsonString.length());
  Serial.print("bytes_sent: ");
  client.write("\n");
  Serial.println(bytes_sent);
  delay(10);
}

JSONVar Movement::measurementToJSON(SensorMeasurement sensorMeasurement) {
  JSONVar jsonAcc;
  jsonAcc["x"] = sensorMeasurement.acc.x;
  jsonAcc["y"] = sensorMeasurement.acc.y;
  jsonAcc["z"] = sensorMeasurement.acc.z;
  JSONVar jsonGyro;
  jsonGyro["x"] = sensorMeasurement.gyro.x;
  jsonGyro["y"] = sensorMeasurement.gyro.y;
  jsonGyro["z"] = sensorMeasurement.gyro.z;
  JSONVar jsonInclination;
  jsonInclination["roll"] = sensorMeasurement.inclination.roll;
  jsonInclination["pitch"] =sensorMeasurement.inclination.pitch;
  jsonInclination["yaw"] = sensorMeasurement.inclination.yaw;
  JSONVar jsonFinger;
  jsonFinger["acc"] = jsonAcc;
  jsonFinger["gyro"] = jsonGyro;
  jsonFinger["inclination"] = jsonInclination;
  return jsonFinger;
}

std::string  Movement::format(float measurement) {
  const int size = 1 + snprintf(NULL, 0, "%.3f", measurement);
  char buff[size];
  snprintf(buff, size, "%.3f", measurement);
  return buff;
}

Movement::~Movement() {}
