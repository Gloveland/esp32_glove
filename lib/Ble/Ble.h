#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "7056f14b-02df-4dd8-86fd-0261c7b15c86"
#define CHARACTERISTIC_UUID_TX "47e62e53-e278-494d-a3f8-ac00973ae0af"

class Ble {
 private:
  BLEServer *profileServer;
  BLEService *profileService;
  BLECharacteristic *profileCharacteristic;
  BLEServerCallbacks *serverCallbacks;
  BLECharacteristicCallbacks *characteristicCallbacks;

 public:
  void init(const std::string &name);
  void notify(const std::string &value);
};

#endif  // BLE_H