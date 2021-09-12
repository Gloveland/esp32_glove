#include "BleCommunicator.h"
#include "ServerCallbacks.h"
#include "TasksControllerCallback.h"

void BleCommunicator::init(const std::string &name,
                           TasksControllerCallback *tasks_controller_callback) {
  this->name_ = name;
  BLEDevice::init(name_);
  profile_server_ = BLEDevice::createServer();
  server_callbacks_ = new ServerCallbacks();
  profile_server_->setCallbacks(server_callbacks_);
  profile_service_ = profile_server_->createService(LSA_GLOVES_SERVICE_UUID);

  //Setup controller characteristic.
  controller_characteristic_ = profile_service_->createCharacteristic(
      CONTROLLER_CHARACTERISTIC_UUID,
          BLECharacteristic::PROPERTY_WRITE);
  controller_characteristic_->addDescriptor(new BLE2902());
  controller_characteristic_->setCallbacks(tasks_controller_callback);

  //Setup data collection characteristic.
  data_collection_characteristic_ = profile_service_->createCharacteristic(
      DATA_COLLECTION_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_NOTIFY);
  data_collection_characteristic_->addDescriptor(new BLE2902());

  //Setup interpretation characteristic.
  interpretation_characteristic_ = profile_service_->createCharacteristic(
      INTERPRETATION_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_NOTIFY);
  interpretation_characteristic_->addDescriptor(new BLE2902());

  profile_service_->start();
  profile_server_->getAdvertising()->start();
}

void BleCommunicator::sendMeasurements(GloveMeasurements measurements) {
  //ImuSensorMeasurement thumb_measurement = measurements.thumb_imu_measurement;
  //TODO(Darius): sendMeasurements the whole glove measurements.
  //notify(JSONVar::stringify(thumb_measurement.toJson()).c_str());
  ImuSensorMeasurement pinky_measurement = measurements.pinky_imu_measurement;
  this->data_collection_characteristic_->setValue(JSONVar::stringify(
      pinky_measurement.toJson()).c_str());
  this->data_collection_characteristic_->notify();
}

void BleCommunicator::sendInterpretation(const std::string& interpretation) {
  this->interpretation_characteristic_->setValue(interpretation);
  this->interpretation_characteristic_->notify();
  Serial.print("Interpretation sent: ");
  Serial.println(interpretation.c_str());
}

BleCommunicator::~BleCommunicator() = default;
