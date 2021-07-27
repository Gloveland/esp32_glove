#ifndef ESP32_GLOVE_SERVERCALLBACKS_H
#define ESP32_GLOVE_SERVERCALLBACKS_H

#include <Arduino.h>
#include <BLEServer.h>

/**
 * Callback to handle server events such as onConnect or onDisconnect.
 */
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) override;
  void onDisconnect(BLEServer *pServer) override;
};

#endif  // ESP32_GLOVE_SERVERCALLBACKS_H