#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <Arduino.h>
#include <BLECharacteristic.h>
#include <Glove.h>

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
  void stopRunningTask() override;
  ~TasksManager();

 private:
  Glove *glove_;
  BleCommunicator *bleCommunicator = nullptr;
  TasksControllerCallback *tasksControllerCallback = nullptr;

  /** Bluetooth service name. */
  static const std::string kBleService_;

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

#endif  // TASK_MANAGER_H
