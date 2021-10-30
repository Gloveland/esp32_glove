
#include "TasksControllerCallback.h"

const std::string TasksControllerCallback::kStartDC_ = "startdc";
const std::string TasksControllerCallback::kStartInterpretation_ = "startint";
const std::string TasksControllerCallback::kStopTask_ = "stop";
const std::string TasksControllerCallback::kCalibrate_ = "calibrate";

TasksControllerCallback::TasksControllerCallback(
    AbstractTasksManager* tasksManager)
    : tasksManager_(tasksManager){};

/** Callback to be called when the device receives a write event. */
void TasksControllerCallback::onWrite(BLECharacteristic* pCharacteristic) {
  BLECharacteristicCallbacks::onWrite(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  this->executeCommand(value);
}
void TasksControllerCallback::executeCommand(std::string command) {
  log_i("Command received: %s", String(command.c_str()));

  if (command == kStopTask_) {
    this->tasksManager_->stopRunningTask();
    return;
  }
  if (command == kStartInterpretation_) {
    this->tasksManager_->startInterpretationTask();
    return;
  }
  if (command == kStartDC_) {
    this->tasksManager_->startDataCollectionTask();
    return;
  }

  if (command == kCalibrate_) {
    this->tasksManager_->startCalibrationTask();
    return;
  }
  log_i("Dropping command: %s ", String(command.c_str()));
}
