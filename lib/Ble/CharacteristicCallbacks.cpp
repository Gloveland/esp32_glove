//
// Created by ekumen on 18/7/21.
//

#include "CharacteristicCallbacks.h"

#include <Arduino.h>

void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  BLECharacteristicCallbacks::onWrite(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  Serial.println(">> onWrite callback run: " + String(value.c_str()));
}

void CharacteristicCallbacks::onNotify(BLECharacteristic *pCharacteristic) {
  BLECharacteristicCallbacks::onNotify(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  Serial.println(">> onNotify callback run: " + String(value.c_str()));
}
