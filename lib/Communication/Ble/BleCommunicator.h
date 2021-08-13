#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <Finger.h>
#include "../../Sensors/ImuSensorMeasurement.h"
#include "../../Sensors/GloveMeasurements.h"

#define SERVICE_UUID            "7056f14b-02df-4dd8-86fd-0261c7b15c86"
#define CHARACTERISTIC_UUID_TX  "47e62e53-e278-494d-a3f8-ac00973ae0af"

/**
 * Class to handle the communication using Bluetooth Low Energy between the
 * NodeMCU board and the mobile application.
 */
class BleCommunicator {
 public:
  BleCommunicator() = default;

  void init(const std::string &name,
            TaskHandle_t &ble_communicator_task_handler);
  void send(GloveMeasurements measurements);
  ~BleCommunicator();

 private:
  void notify(const std::string &value);
  std::string name_;
  TaskHandle_t ble_communicator_task_handler_{};
  BLEServer *profile_server_{};
  BLEService *profile_service_{};
  BLECharacteristic *profile_characteristic_{};
  BLEServerCallbacks *server_callbacks_{};
  BLECharacteristicCallbacks *characteristic_callbacks_{};
};

#endif  // BLE_H
