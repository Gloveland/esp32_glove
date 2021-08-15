
#include "ServerCallbacks.h"

void ServerCallbacks::onConnect(BLEServer *pServer) {
  BLEServerCallbacks::onConnect(pServer);
  Serial.println("********* Device Connected :) ************");
}

void ServerCallbacks::onDisconnect(BLEServer *pServer) {
  BLEServerCallbacks::onDisconnect(pServer);
  Serial.println("********* Device Disconnected :( ************");
}
