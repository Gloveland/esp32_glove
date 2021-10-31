#ifndef ESP32_GLOVE_SERVERCALLBACKS_H
#define ESP32_GLOVE_SERVERCALLBACKS_H

#include <Arduino.h>
#include <BLEServer.h>
#include "BleCommunicator.h"
#include "TasksControllerCallback.h"

/**
 * Callback to handle server events such as onConnect or onDisconnect.
 */
class ServerCallbacks : public BLEServerCallbacks {

  public:
    ServerCallbacks(BleCommunicator * communicator, BLECharacteristicCallbacks * tasks_controller_callback);
    void onConnect(BLEServer *pServer) override;
    void onDisconnect(BLEServer *pServer) override;
    ~ServerCallbacks();
  private:
    BleCommunicator * communicator_;
    TasksControllerCallback * tasks_controller_callback_;
};

#endif  // ESP32_GLOVE_SERVERCALLBACKS_H
