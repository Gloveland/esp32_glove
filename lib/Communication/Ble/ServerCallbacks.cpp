
#include "ServerCallbacks.h"

ServerCallbacks::ServerCallbacks(
    BleCommunicator *communicator,
    TasksControllerCallback *tasks_controller_callback)
    : BLEServerCallbacks(),
      communicator_(communicator),
      tasks_controller_callback_(tasks_controller_callback) {}

void ServerCallbacks::onConnect(BLEServer *pServer) {
  BLEServerCallbacks::onConnect(pServer);
  Serial.println("********* Device Connected :) ************");
}

void ServerCallbacks::onDisconnect(BLEServer *pServer) {
  BLEServerCallbacks::onDisconnect(pServer);
  Serial.println("********* Device Disconnected :( ************");
  this->tasks_controller_callback_->stopRunningTask();
  this->communicator_->advertiseAgain();
}

ServerCallbacks::~ServerCallbacks() = default;
