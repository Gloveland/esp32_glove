#ifndef ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H
#define ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H

#include <BLECharacteristic.h>
#include <Glove.h>

#include "Glove.h"
#include "BleCommunicator.h"
#include "Counter.h"

/**
 * Callbacks to handle characteristic events such as onWrite.
 */
class TasksControllerCallback : public BLECharacteristicCallbacks {
 public:
  TasksControllerCallback(Glove * glove);

  void init();


  void onWrite(BLECharacteristic *pCharacteristic) override;

  /**
   * Stop the ongoing task, either the interpretation task or the data
   * collection task. Nothing happens if there is not an ongoing task.
   */
  void stopRunningTask();

 private:
  Glove * glove_;
  BleCommunicator * bleCommunicator;

  /** Bluetooth service name. */
  static const std::string kBleService_;
  /** Command to start the data collection task. */
  static const std::string kStartDC_;

  /** Command to start the interpretation task. */
  static const std::string kStartInterpretation_;

  /** Command to stop the running task */
  static const std::string kStopTask_;

  /** Command to calibrate the glove sensors. */
  static const std::string kCalibrate_;

  /** Starts the data collection task. */
  void startDataCollectionTask();

  /**
   * The task handler of the running task. If neither the interpretation task
   * nor the data collection task are running then this value is null.
   */
  TaskHandle_t running_task_handler_;

  /**
   * Handler of the Read sensors task. The task associated to this
   * handler will be suspended upon receiving a command and restarted
   * when receiving a "start" command.
   */
  TaskHandle_t data_collection_task_handler_;

  /** Starts the data collection task. */
  static void startDataCollectionTaskImpl(void *_this);
  [[noreturn]] void taskDataCollection();
};

#endif  // ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H
