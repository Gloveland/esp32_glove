
#include "TasksControllerCallback.h"

#include <Arduino.h>

const std::string TasksControllerCallback::kBleService_ = "RightHandSmartGlove";
const std::string TasksControllerCallback::kStartDC_ = "startdc";
const std::string TasksControllerCallback::kStartInterpretation_ = "startint";
const std::string TasksControllerCallback::kStopTask_ = "stop";
const std::string TasksControllerCallback::kCalibrate_ = "calibrate";

TasksControllerCallback::TasksControllerCallback(Glove* glove)
    : glove_(glove), running_task_handler_(nullptr) {
  log_i("Create task controller callbakc");
  this->bleCommunicator = new BleCommunicator();
};

void TasksControllerCallback::init() {
  log_i("init ble communicator");
  this->bleCommunicator->init(TasksControllerCallback::kBleService_, this);
}

/** Callback to be called when the device receives a write event. */
void TasksControllerCallback::onWrite(BLECharacteristic* pCharacteristic) {
  BLECharacteristicCallbacks::onWrite(pCharacteristic);
  std::string value = pCharacteristic->getValue();
  log_i("Command received: %s", String(value.c_str()));
  if (value == kStopTask_) {
    stopRunningTask();
    return;
  }
  if (value == kStartDC_) {
    startDataCollectionTask();
    return;
  }
  log_i("Dropping command: %s ", String(value.c_str()));
}

void TasksControllerCallback::startDataCollectionTask() {
  this->stopRunningTask();
  log_i("Starting data collection task.");
  xTaskCreatePinnedToCore(
      this->startDataCollectionTaskImpl,  // Task function
      "readSensors",                      // Name of the task
      10000,                              // Stack size of task
      this,                               // Parameter of the task
      1,                                  // Priority of the task
      &data_collection_task_handler_,     // Task handle to keep
                                          // track of created task
      0                                   // Pin task to core 0
  );
  this->running_task_handler_ = this->data_collection_task_handler_;
}

void TasksControllerCallback::startDataCollectionTaskImpl(void* _this) {
  log_i("taskDataCollection...");
  if (_this == nullptr) {
    log_i("Error creating task, null task parameter");
  }
  ((TasksControllerCallback*)_this)->taskDataCollection();
}

[[noreturn]] void TasksControllerCallback::taskDataCollection() {
  Counter counter;
  for (;;) {
    float elapsedTime = counter.getAndUpdateElapsedTimeSinceLastMeasurementMs();
    log_i("frequency: %.3f hz",1.0 / (elapsedTime / 1000.0));  // Divide by 1000 to get seconds
    GloveMeasurements measurements = this->glove_->readSensors();
    int count = counter.getAndUpdateCounter();
    std::string pkg = measurements.toPackage(count, elapsedTime);
    this->bleCommunicator->sendMeasurements(pkg);
    log_i("Counter: %d", count);
    log_i("Elapsed time: %f", elapsedTime);
  }
}

void TasksControllerCallback::stopRunningTask() {
  if (running_task_handler_ != nullptr) {
    log_i("Stopping running task.");
    vTaskDelete(running_task_handler_);
    running_task_handler_ = nullptr;
    log_i("Task stopped.");
    return;
  }
  log_i("Dropping stop command: no task was running.");
}
