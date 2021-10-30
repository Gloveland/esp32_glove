#ifndef TASKS_CONTROLLER_CALLBACKS_H
#define TASKS_CONTROLLER_CALLBACKS_H

#include <Arduino.h>
#include <BLECharacteristic.h>

#include "AbstractTasksManager.h"

/**
 * Callbacks to handle characteristic events such as onWrite.
 */
class TasksControllerCallback : public BLECharacteristicCallbacks {
 public:
  TasksControllerCallback(AbstractTasksManager* tasksManager);

  void onWrite(BLECharacteristic* pCharacteristic) override;

  void executeCommand(std::string command);

  /** Command to start the data collection task. */
  static const std::string kStartDC_;

  /** Command to start the interpretation task. */
  static const std::string kStartInterpretation_;

  /** Command to stop the running task */
  static const std::string kStopTask_;

  /** Command to calibrate the glove sensors. */
  static const std::string kCalibrate_;

 private:
  AbstractTasksManager* tasksManager_;
};

#endif  // TASKS_CONTROLLER_CALLBACKS_H
