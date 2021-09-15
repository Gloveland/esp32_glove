#include "BleCommunicator.h"
#include "ServerCallbacks.h"
#include "TasksControllerCallback.h"

const std::string BleCommunicator::kglove_mesurements_packet_format = "%d\nP%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nR%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nM%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nI%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\nT%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f;";

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
  ImuSensorMeasurement pinky = *measurements.pinky_imu_measurement;
  ImuSensorMeasurement ring = *measurements.ring_imu_measurement;
  ImuSensorMeasurement middle = *measurements.middle_imu_measurement;
  ImuSensorMeasurement index = *measurements.index_imu_measurement;
  ImuSensorMeasurement thump = *measurements.thumb_imu_measurement;

  int size_written =  sprintf(glove_measurement_buffer, BleCommunicator::kglove_mesurements_packet_format.c_str(),
          this->events_count_,
          pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(), pinky.getInclination().getRoll(),  pinky.getInclination().getPitch(),  pinky.getInclination().getYaw(),
          pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(), pinky.getInclination().getRoll(),  pinky.getInclination().getPitch(),  pinky.getInclination().getYaw(),
          pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(), pinky.getInclination().getRoll(),  pinky.getInclination().getPitch(),  pinky.getInclination().getYaw(),
          pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(), pinky.getInclination().getRoll(),  pinky.getInclination().getPitch(),  pinky.getInclination().getYaw(),
          pinky.getAcc().getX(), pinky.getAcc().getY(), pinky.getAcc().getZ(), pinky.getGyro().getX(), pinky.getGyro().getY(), pinky.getGyro().getZ(), pinky.getInclination().getRoll(),  pinky.getInclination().getPitch(),  pinky.getInclination().getYaw());
  this->events_count_ += 1;

  Serial.print("size_written: "); Serial.println(size_written);
  if(size_written > this->kmtu){
    Serial.print("Error size is bigger than 512!!");
  }    
  Serial.println(String(glove_measurement_buffer));
  this->data_collection_characteristic_->setValue(glove_measurement_buffer);
  this->data_collection_characteristic_->notify();

}

void BleCommunicator::sendInterpretation(const std::string& interpretation) {
  this->interpretation_characteristic_->setValue(interpretation);
  this->interpretation_characteristic_->notify();
  Serial.print("Interpretation sent: ");
  Serial.println(interpretation.c_str());
}

BleCommunicator::~BleCommunicator() = default;
