//
// Created by Jazmin Ferreiro on 22/07/2021.
//

#include "Movement.h"
Movement::Movement(int num, string id, Hand hand) :
    eventNum(num),
    deviceId(id),
    hand(hand) {

}
void Movement::send(WiFiClient client) {
  const int buffer_size =
      1 + snprintf(NULL, 0, FORMATTED_JSON_MEASUREMENT,
                   this->deviceId.c_str(),
                   this->eventNum, this->hand.thump.acc.x, this->hand.thump.acc.y,
                   this->hand.thump.acc.z, this->hand.thump.gyro.x,
                   this->hand.thump.gyro.y, this->hand.thump.gyro.z,
                   this->hand.thump.inclination.roll,
                   this->hand.thump.inclination.pitch,
                   this->hand.thump.inclination.yaw);
  Serial.print("  buffer_size: ");
  Serial.
      print(buffer_size);
  assert(buffer_size
             > 0);
  char package[buffer_size];
  int size_written = snprintf(
      package, buffer_size, FORMATTED_JSON_MEASUREMENT,
      this->deviceId.c_str(),
      this->eventNum, this->hand.thump.acc.x, this->hand.thump.acc.y,
      this->hand.thump.acc.z, this->hand.thump.gyro.x,
      this->hand.thump.gyro.y, this->hand.thump.gyro.z,
      this->hand.thump.inclination.roll,
      this->hand.thump.inclination.pitch,
      this->hand.thump.inclination.yaw);
  assert(size_written
             == buffer_size - 1);
  Serial.print("  size_written: ");
  Serial.
      print(size_written);
  Serial.print("  sending value via wifi: ");
  Serial.
      println(package);
  int bytes_sent = client.write(package, size_written);
  Serial.print("bytes_sent: ");
  Serial.println(bytes_sent);
  delay(10);
}

Movement::~Movement() {

}
