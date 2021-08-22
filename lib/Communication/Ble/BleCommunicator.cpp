#include "BleCommunicator.h"
#include "ServerCallbacks.h"
#include "CharacteristicCallbacks.h"

void BleCommunicator::init(const std::string &name,
                           TaskHandle_t &ble_communicator_task_handler) {
  this->name_ = name;
  this->ble_communicator_task_handler_ = ble_communicator_task_handler;
  BLEDevice::init(name_);
  profile_server_ = BLEDevice::createServer();
  server_callbacks_ = new ServerCallbacks();
  profile_server_->setCallbacks(server_callbacks_);
  profile_service_ = profile_server_->createService(SERVICE_UUID);
  profile_characteristic_ = profile_service_->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_WRITE);
  profile_characteristic_->addDescriptor(new BLE2902());
  characteristic_callbacks_ =
      new CharacteristicCallbacks(ble_communicator_task_handler_);
  profile_characteristic_->setCallbacks(characteristic_callbacks_);
  profile_service_->start();
  profile_server_->getAdvertising()->start();
}

void BleCommunicator::notify(const std::string &value) {
  this->profile_characteristic_->setValue(value);
  this->profile_characteristic_->notify();
  Serial.println();
  Serial.print("BLECommunicator sent: ");
  Serial.println(value.c_str());
}

BleCommunicator::~BleCommunicator() = default;

void BleCommunicator::send(GloveMeasurements measurements) {
  //ImuSensorMeasurement thumb_measurement = measurements.thumb_imu_measurement;
  //TODO(Darius): send the whole glove measurements.
  //notify(JSONVar::stringify(thumb_measurement.toJson()).c_str());
  ImuSensorMeasurement pinky_measurement = measurements.pinky_imu_measurement;
  notify(JSONVar::stringify(pinky_measurement.toJson()).c_str());
}
