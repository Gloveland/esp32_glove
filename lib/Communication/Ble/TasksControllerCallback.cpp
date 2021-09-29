
#include "TasksControllerCallback.h"

#include <Arduino.h>

/** Callback to be called when the device receives a write event. */
void TasksControllerCallback::onWrite(BLECharacteristic *pCharacteristic) {
  BLECharacteristicCallbacks::onWrite(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  Serial.println("[TasksControllerCallback] Command received: "
                     + String(value.c_str()));

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
    startCalibration();
    return;
  }

  Serial.println("[TasksControllerCallback] Dropping command: "
                     + String(value.c_str()));
}

void TasksControllerCallback::startDataCollectionTask() {
  if (this->data_collection_task_handler_ != nullptr) {
    stopRunningTask();
    running_task_handler_ = data_collection_task_handler_;
    Serial.println("[TasksControllerCallback] Starting data collection task.");
    vTaskResume(running_task_handler_);
  }
}

void TasksControllerCallback::startInterpretationTask() {
  if (this->interpretation_task_handler_ != nullptr) {
    stopRunningTask();
    running_task_handler_ = interpretation_task_handler_;
    Serial.println("[TasksControllerCallback] Starting interpretation task.");
    vTaskResume(running_task_handler_);
  }
}

void TasksControllerCallback::startCalibration() {
  if (this->calibration_task_handler_ != nullptr) {
    stopRunningTask();
    running_task_handler_ = calibration_task_handler_;
    Serial.println("[TasksControllerCallback] Starting calibration.");
    vTaskResume(running_task_handler_);
  }
}

void TasksControllerCallback::stopRunningTask() {
  if (running_task_handler_ != nullptr) {
    Serial.println("[TasksControllerCallback] Stopping running task.");
    vTaskSuspend(running_task_handler_);
    running_task_handler_ = nullptr;
    Serial.println("[TasksControllerCallback] Task stopped.");
    return;
  }
  Serial.println(
      "[TasksControllerCallback] Dropping stop command: no task was running.");
}
