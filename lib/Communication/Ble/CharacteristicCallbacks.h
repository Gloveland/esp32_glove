#ifndef ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H
#define ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H

#include <BLECharacteristic.h>

/**
 * Callbacks to handle characteristic events such as onWrite.
 */
class CharacteristicCallbacks : public BLECharacteristicCallbacks {
 public:
  explicit CharacteristicCallbacks(TaskHandle_t read_sensors_task_handler)
      : read_sensors_task_handler_(read_sensors_task_handler) {}

  void onWrite(BLECharacteristic* pCharacteristic) override;

 private:
  /**
   * Handler of the Read sensors task. The task associated to this 
   * handler will be suspended upon receiving a command and restarted 
   * when receiving a "start" command.
   */
  TaskHandle_t read_sensors_task_handler_;

  /** Command to start the read sensor task. */
  const std::string kStart_ = "start";

  /** Command to stop the read sensor task. */
  const std::string kStop_ = "stop";
};

#endif  // ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H
