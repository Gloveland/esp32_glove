#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <Arduino.h>
#include <BLECharacteristic.h>
#include <Glove.h>
#include <GloveMeasurements.h>

#include <sstream>

#include "../Communication/Ble/BleCommunicator.h"
#include "../Communication/Ble/TasksControllerCallback.h"
#include "Counter.h"

/**
 * Callbacks to handle characteristic events such as onWrite.
 */
class TasksManager : public AbstractTasksManager {
 public:
  TasksManager(Glove *glove);

  void initBleService() override;
  void startDataCollectionTask() override;
  void startBleMeasurementSenderTask() override;
  void startInterpretationTask() override;
  void startCalibrationTask() override;
  void stopRunningTask() override;
  ~TasksManager();

 private:
  Glove *glove_;
  QueueHandle_t queue = nullptr;
  BleCommunicator *bleCommunicator = nullptr;
  TasksControllerCallback *tasksControllerCallback = nullptr;

  /** Bluetooth service name. */
  static const std::string kBleService_;
  static const int kQueueSize;
  static const int kHightPriority;
  static const int kLowPriority;

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

  /** Handler of the ble mesurement task. */
  TaskHandle_t ble_measurement_sender_task_handler_;

  /** Handler of the interpretation task. */
  TaskHandle_t interpretation_task_handler_;

  /** Handler of the calibration task. */
  TaskHandle_t calibration_task_handler_;

  /** Starts the data collection task. */
  static void startDataCollectionTaskImpl(void *_this);
  [[noreturn]] void taskDataCollection();

  /** Starts the interpretation task. */
  static void startInterpretationTaskImpl(void *_this);
  [[noreturn]] void taskInterpretation();

  /** Starts the measurement sender task. */
  static void startBleMeasurementSenderTaskImpl(void *_this);
  [[noreturn]] void taskBleMeasurementSender();

  /** Starts the calibration task. */
  static void startCalibrationTaskImpl(void *_this);
  [[noreturn]] void taskCalibration();
};

#endif  // TASK_MANAGER_H
