//
// Created by ekumen on 18/7/21.
//

#include "ServerCallbacks.h"

void ServerCallbacks::onConnect(BLEServer *pServer) {
  BLEServerCallbacks::onConnect(pServer);
  Serial.println("********* Device Connected :) ************");
}

void ServerCallbacks::onDisconnect(BLEServer *pServer) {
  BLEServerCallbacks::onDisconnect(pServer);
  Serial.println("********* Device Disconnected :( ************");
}
