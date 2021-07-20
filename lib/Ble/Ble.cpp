#include <Ble.h>

#include "CharacteristicCallbacks.h"
#include "ServerCallbacks.h"

void Ble::init(const std::string& name) {
  BLEDevice::init(name);
  this->serverCallbacks = new ServerCallbacks();

  // create ble server
  this->profileServer = BLEDevice::createServer();
  this->profileServer->setCallbacks(this->serverCallbacks);

  // create service
  this->profileService = this->profileServer->createService(SERVICE_UUID);

  // create characteristic
  this->profileCharacteristic = this->profileService->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE);
  this->profileCharacteristic->addDescriptor(new BLE2902());
  this->characteristicCallbacks = new CharacteristicCallbacks();
  this->profileCharacteristic->setCallbacks(characteristicCallbacks);
  // this->profileCharacteristic->setValue("Hola soy jaz desde el esp32");

  this->profileService->start();
  this->profileServer->getAdvertising()->start();
}

void Ble::notify(const std::string& value) {
  this->profileCharacteristic->setValue(value);
  this->profileCharacteristic->notify();
  Serial.println("send value: " + String(value.c_str()));
}