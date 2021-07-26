#include "Movement.h"

Movement::Movement(int num, string id, HandMov hand)
    : eventNum(num), deviceId(id), hand(hand) {}

void Movement::send(WiFiClient client) {
  JSONVar jsonHand;
  //jsonHand["index"] = this->getFingerJson(this->hand.index);
  //jsonHand["middle"] = this->getFingerJson(this->hand.middle);
  //jsonHand["ring"] = this->getFingerJson(this->hand.ring);
  //jsonHand["pinky"] = this->getFingerJson(this->hand.pinky);
  JSONVar thump = this->getFingerJson(this->hand.thump);

  jsonHand["thump"] = thump;
  JSONVar jsonMovement;
  jsonMovement["device_id"] = this->deviceId.c_str();
  jsonMovement["event_num"] = this->eventNum;
  jsonMovement["hand"] = jsonHand;
  Serial.print("  sending value via wifi: ");
  String jsonString = JSON.stringify(jsonMovement);
  Serial.println(jsonString);
  int bytes_sent = client.write(jsonString.c_str(), jsonString.length());
  Serial.print("bytes_sent: ");
  Serial.println(bytes_sent);
  delay(10);
}

JSONVar Movement::getFingerJson(FingerMov finger) {
  JSONVar jsonAcc;
  jsonAcc["x"] = this->format(finger.acc.x).c_str();
  jsonAcc["y"] = this->format(finger.acc.y).c_str();
  jsonAcc["z"] = this->format(finger.acc.z).c_str();
  JSONVar jsonGyro;
  jsonGyro["x"] = this->format(finger.gyro.x).c_str();
  jsonGyro["y"] = this->format(finger.gyro.y).c_str();
  jsonGyro["z"] = this->format(finger.gyro.z).c_str();
  JSONVar jsonInclination;
  jsonInclination["roll"] = this->format(finger.inclination.roll).c_str();
  jsonInclination["pith"] = this->format(finger.inclination.pitch).c_str();
  jsonInclination["yaw"] = this->format(finger.inclination.yaw).c_str();
  JSONVar jsonFinger;
  jsonFinger["acc"] = jsonAcc;
  jsonFinger["gyro"] = jsonGyro;
  jsonFinger["inclination"] = jsonInclination;
  return jsonFinger;
}

string Movement::format(float measurement) {
  const int size = 1 + snprintf(NULL, 0, "%.3f", measurement);
  char buff[size];
  snprintf(buff, size, "%.3f", measurement);
  return buff;
}

Movement::~Movement() {}
