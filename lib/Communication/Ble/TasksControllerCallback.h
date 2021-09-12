#ifndef ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H
#define ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H

#include <BLECharacteristic.h>

/**
 * Callbacks to handle characteristic events such as onWrite.
 */
class TasksControllerCallback : public BLECharacteristicCallbacks {
 public:
  explicit TasksControllerCallback(TaskHandle_t &data_collection_task_handler,
                                   TaskHandle_t &interpretation_task_handler)
      : data_collection_task_handler_(data_collection_task_handler),
        interpretation_task_handler_(interpretation_task_handler) {}

  void onWrite(BLECharacteristic *pCharacteristic) override;

 private:
  /**
   * The task handler of the running task. If neither the interpretation task
   * nor the data collection task are running then this value is null.
   */
  TaskHandle_t running_task_handler_ = nullptr;

  /**
   * Handler of the Read sensors task. The task associated to this 
   * handler will be suspended upon receiving a command and restarted 
   * when receiving a "start" command.
   */
  TaskHandle_t &data_collection_task_handler_;

  /** Handler of the interpretation task. */
  TaskHandle_t &interpretation_task_handler_;

  /** Command to start the data collection task. */
  const std::string kStartDC_ = "startdc";

  /** Command to start the interpretation task. */
  const std::string kStartInterpretation_ = "startint";

  /**
   * Command to stop the running task, either the data collection task or the
   * interpretation task.
   */
  const std::string kStopTask_ = "stop";

  /** Starts the data collection task. */
  void startDataCollectionTask();

  /** Starts the interpretation task. */
  void startInterpretationTask();

  /**
   * Stop the ongoing task, either the interpretation task or the data
   * collection task. Nothing happens if there is not an ongoing task.
   */
  void stopRunningTask();
};

#endif  // ESP32_GLOVE_CHARACTERISTIC_CALLBACKS_H