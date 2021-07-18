#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "7056f14b-02df-4dd8-86fd-0261c7b15c86"
#define CHARACTERISTIC_UUID_TX "47e62e53-e278-494d-a3f8-ac00973ae0af"
#define ACK_SIZE 4
#define ACK_LIMIT 10
#define ACK_INIT 1

class Ble : public BLEServerCallbacks {
 public:
  Ble();
  void init(const std::string);
  void onConnect(BLEServer *pServer);
  void onDisconnect(BLEServer *pServer);
  void notifyWithAck(const std::string value);
  void notify(const std::string value);
  void indicate(const std::string value);
  ~Ble();

 private:
  int ack_count;
  std::string name;
  BLEServer *profileServer;
  BLEService *profileService;
  BLECharacteristic *profileCharacteristic;
};

#endif  // BLE_H