#ifndef ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H
#define ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H

#include <BLECharacteristic.h>

/**
 * Callbacks to handle characteristic events such as onWrite or onNotify.
 */
class CharacteristicCallbacks : public BLECharacteristicCallbacks {
 public:
  explicit CharacteristicCallbacks(TaskHandle_t taskHandler)
      : task_handler_(taskHandler) {}

  void onWrite(BLECharacteristic* pCharacteristic) override;

 private:
  TaskHandle_t task_handler_;
  const std::string kStop_ = "stop";
  const std::string kStart_ = "start";
};

#endif  // ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H
