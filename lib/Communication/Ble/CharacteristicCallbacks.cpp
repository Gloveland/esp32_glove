
#include "CharacteristicCallbacks.h"

#include <Arduino.h>

/**
 * Callback to be called when the device receives a write event.
 *
 * <p>Upon receiving 'start' the read sensors task will be started and will be
 * suspended when receiving a 'stop' command.
 */
void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  BLECharacteristicCallbacks::onWrite(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  Serial.println("[CharacteristicCallbacks] \"onWrite\" callback run: "
                     + String(value.c_str()));
  if (value == kStop_ && read_sensors_task_handler_ != nullptr) {
    Serial.println(
        "[CharacteristicCallbacks] Received stop. Suspending read sensor task.");
    vTaskSuspend(read_sensors_task_handler_);
    return;
  }

  if (value == kStart_ && read_sensors_task_handler_ != nullptr) {
    vTaskResume(read_sensors_task_handler_);
  }
}
