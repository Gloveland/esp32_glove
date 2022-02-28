#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include <BLECharacteristic.h>
#include <Glove.h>
#include <sstream>

#include "../Communication/Ble/BleCommunicator.h"
#include "../Communication/Ble/TasksControllerCallback.h"
#include "../Interpretation/Interpreter.h"
#include "Counter.h"

class TasksControllerCallback;

/**
 * Callbacks to handle characteristic events such as onWrite.
 */
class TasksManager {
 public:
  TasksManager(Glove *glove);

  /** Init Bluetooth low energy services. */
  void initBleService();

  /** Starts the data collection task. */
  void startDataCollectionTask();

  /** Starts the interpretation task. */
  void startInterpretationTask();

  /** Starts the calibration task. */
  void startCalibrationTask();

  /**
   * Stop the ongoing task, either the interpretation task or the data
   * collection task. Nothing happens if there is not an ongoing task.
   */
  void stopRunningTask();
  
  ~TasksManager();

 private:
  Glove *glove_;
  BleCommunicator *bleCommunicator = nullptr;
  TasksControllerCallback *tasksControllerCallback = nullptr;
  Interpreter *interpreter = nullptr;
  
  /** Bluetooth service name. */
  static const std::string kBleService_;

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
  TaskHandle_t data_collection_task_handler_;

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

  /** Starts the calibration task. */
  static void startCalibrationTaskImpl(void *_this);
  [[noreturn]] void taskCalibration();
};

#endif  // TASK_MANAGER_H
