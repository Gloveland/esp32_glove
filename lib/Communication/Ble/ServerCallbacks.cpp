
#include "ServerCallbacks.h"

ServerCallbacks::ServerCallbacks(
    BleCommunicator *communicator,
    BLECharacteristicCallbacks *tasks_controller_callback)
    : BLEServerCallbacks(),
      communicator_(communicator),
      tasks_controller_callback_((TasksControllerCallback*)tasks_controller_callback) {}

void ServerCallbacks::onConnect(BLEServer *pServer) {
  BLEServerCallbacks::onConnect(pServer);
  log_i("********* Device Connected :) ************");
}

void ServerCallbacks::onDisconnect(BLEServer *pServer) {
  BLEServerCallbacks::onDisconnect(pServer);
  log_i("********* Device Disconnected :( ************");
  this->tasks_controller_callback_->stopRunningTask();
  this->communicator_->advertiseAgain();
}

ServerCallbacks::~ServerCallbacks() = default;
