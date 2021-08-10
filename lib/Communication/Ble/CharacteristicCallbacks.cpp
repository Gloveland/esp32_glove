
#include "CharacteristicCallbacks.h"

#include <Arduino.h>

void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  BLECharacteristicCallbacks::onWrite(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  Serial.println(">> onWrite callback run: " + String(value.c_str()));
  if (value == kStop_ && task_handler_ != nullptr) {
    Serial.println(">> Received stop");
    vTaskSuspend(task_handler_);
    return;
  }

  if (value == kStart_ && task_handler_ != nullptr) {
    vTaskResume(task_handler_);
  }
}