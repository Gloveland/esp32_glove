
#include "TasksControllerCallback.h"

#include <Arduino.h>

/** Callback to be called when the device receives a write event. */
void TasksControllerCallback::onWrite(BLECharacteristic *pCharacteristic) {
  BLECharacteristicCallbacks::onWrite(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  log_i("Command received: %s", String(value.c_str()));

  if (value == kStopTask_) {
    stopRunningTask();
    return;
  }

  if (value == kStartInterpretation_) {
    startInterpretationTask();
    return;
  }

  if (value == kStartDC_) {
    startDataCollectionTask();
    return;
  }

  if (value == kCalibrate_) {
    startCalibrationTask();
    return;
  }

  log_i("Dropping command: %s ", String(value.c_str()));
}

void TasksControllerCallback::startDataCollectionTask() {
  if (this->data_collection_task_handler_ != nullptr) {
    stopRunningTask();
    this->counter_->resetCounter();
    running_task_handler_ = data_collection_task_handler_;
    log_i("Starting data collection task.");
    vTaskResume(running_task_handler_);
  }
}

void TasksControllerCallback::startInterpretationTask() {
  if (this->interpretation_task_handler_ != nullptr) {
    stopRunningTask();
    running_task_handler_ = interpretation_task_handler_;
    log_i("Starting interpretation task.");
    vTaskResume(running_task_handler_);
  }
}

void TasksControllerCallback::startCalibrationTask() {
  if (this->calibration_task_handler_ != nullptr) {
    stopRunningTask();
    running_task_handler_ = calibration_task_handler_;
    log_i("Starting calibration.");
    vTaskResume(running_task_handler_);
  }
}

void TasksControllerCallback::stopRunningTask() {
  if (running_task_handler_ != nullptr) {
    log_i("Stopping running task.");
    vTaskSuspend(running_task_handler_);
    this->counter_->resetCounter();
    running_task_handler_ = nullptr;
    log_i("Task stopped.");
    return;
  }
  log_i("Dropping stop command: no task was running.");
}
