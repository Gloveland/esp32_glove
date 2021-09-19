#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <Finger.h>
#include "BleSpecification.h"
#include "../../Sensors/ImuSensorMeasurement.h"
#include "../../Sensors/GloveMeasurements.h"
#include "TasksControllerCallback.h"

/**
 * Class to handle the communication using Bluetooth Low Energy between the
 * NodeMCU board and the mobile application.
 */
class BleCommunicator {
 public:
  BleCommunicator() = default;

  void init(const std::string &name,
            TasksControllerCallback *tasks_controller_callback);
  void sendMeasurements(GloveMeasurements measurements);
  void sendInterpretation(const std::string &interpretation);
  void advertiseAgain();
  ~BleCommunicator();

 private:
  const static int kMtu = 512;
  char glove_measurement_buffer_[kMtu];
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
