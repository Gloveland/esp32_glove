#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "BleSpecification.h"
#include "GloveMeasurements.h"

/**
 * Class to handle the communication using Bluetooth Low Energy between the
 * NodeMCU board and the mobile application.
 */
class BleCommunicator {
 public:
  BleCommunicator() = default;

  void init(const std::string &name,
            BLECharacteristicCallbacks *tasks_controller_callback);
  void sendMeasurements(const std::string measurement);
  void sendInterpretation(const std::string &interpretation);
  void advertiseAgain();
  ~BleCommunicator();

 private:
  long events_count_;
  std::string name_;
  BLEServer *profile_server_{};
  BLEService *profile_service_{};
  BLECharacteristic *controller_characteristic_{};
  BLECharacteristic *data_collection_characteristic_{};
  BLECharacteristic *interpretation_characteristic_{};
  BLEServerCallbacks *server_callbacks_{};
};

#endif  // BLE_H
