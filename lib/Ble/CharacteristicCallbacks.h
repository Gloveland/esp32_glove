#ifndef ESP32_GLOVE_CHARACTERISTICCALLBACKS_H
#define ESP32_GLOVE_CHARACTERISTICCALLBACKS_H

#include <BLECharacteristic.h>

/**
 * Callbacks to handle characteristic events such as onWrite or onNotify.
 */
class CharacteristicCallbacks : public BLECharacteristicCallbacks {
 private:
  TaskHandle_t taskHandler;

 public:
  explicit CharacteristicCallbacks(TaskHandle_t taskHandler)
      : taskHandler(taskHandler) {}

  void onWrite(BLECharacteristic* pCharacteristic) override;
};

#endif  // ESP32_GLOVE_CHARACTERISTICCALLBACKS_H
