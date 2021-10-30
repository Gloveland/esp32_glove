
#include "TasksManager.h"

const std::string TasksManager::kBleService_ = "RightHandSmartGlove";

TasksManager::TasksManager(Glove* glove)
    : glove_(glove), running_task_handler_(nullptr) {
  log_i("Create task controller callbakc");
  this->bleCommunicator = new BleCommunicator();
  this->tasksControllerCallback = new TasksControllerCallback(this);
};

void TasksManager::initBleService() {
  log_i("init ble communicator");
  this->bleCommunicator->init(TasksManager::kBleService_,
                              this->tasksControllerCallback);
}

void TasksManager::startDataCollectionTask() {
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

void TasksManager::startDataCollectionTaskImpl(void* _this) {
  log_i("taskDataCollection...");
  if (_this == nullptr) {
    log_i("Error creating task, null task parameter");
  }
  ((TasksManager*)_this)->taskDataCollection();
}

[[noreturn]] void TasksManager::taskDataCollection() {
  Counter counter;
  for (;;) {
    float elapsedTime = counter.getAndUpdateElapsedTimeSinceLastMeasurementMs();
    log_i("frequency: %.3f hz",
          1.0 / (elapsedTime / 1000.0));  // Divide by 1000 to get seconds
    GloveMeasurements measurements = this->glove_->readSensors();
    int count = counter.getAndUpdateCounter();
    std::string pkg = measurements.toPackage(count, elapsedTime);
    this->bleCommunicator->sendMeasurements(pkg);
    log_i("Counter: %d", count);
    log_i("Elapsed time: %f", elapsedTime);
  }
}

void TasksManager::startInterpretationTask() {
  this->stopRunningTask();
  log_i("Starting interpretation task.");
  xTaskCreatePinnedToCore(
      this->startInterpretationTaskImpl,  // Task function
      "Interpretation task",              // Name of the task
      10000,                              // Stack size of task
      this,                               // Parameter of the task
      1,                                  // Priority of the task
      &interpretation_task_handler_,  // Task handle to keep track of created
                                      // task
      0                               // Pin task to core 0
  );
  this->running_task_handler_ = this->interpretation_task_handler_;
}

void TasksManager::startInterpretationTaskImpl(void* _this) {
  log_i("taskDataCollection...");
  if (_this == nullptr) {
    log_i("Error creating task, null task parameter");
  }
  ((TasksManager*)_this)->taskInterpretation();
}

[[noreturn]] void TasksManager::taskInterpretation() {
  log_i("Task 'Interpretation' running on core %d", xPortGetCoreID());
  for (;;) {
    for (int i = 0; i < 5; i++) {
      // TODO(https://git.io/Ju6I7): Implement translation task.
      std::stringstream mock_interpretation_stream;
      mock_interpretation_stream << "Interpretation " << i;
      this->bleCommunicator->sendInterpretation(
          mock_interpretation_stream.str());
    }
  }
}

void TasksManager::startCalibrationTask() {
  this->stopRunningTask();
  log_i("Starting calibration task.");
  xTaskCreatePinnedToCore(this->startCalibrationTaskImpl,  // Task function
                          "Calibration task",          // Name of the task
                          10000,                       // Stack size of task
                          this,                        // Parameter of the task
                          1,                           // Priority of the task
                          &calibration_task_handler_,  // Task handle to keep
                                                       // track of created task
                          0  // Pin task to core 0
  );
  this->running_task_handler_ = this->calibration_task_handler_;
}

void TasksManager::startCalibrationTaskImpl(void* _this) {
  log_i("taskDataCollection...");
  if (_this == nullptr) {
    log_i("Error creating task, null task parameter");
  }
  ((TasksManager*)_this)->taskCalibration();
}

[[noreturn]] void TasksManager::taskCalibration() {
  log_i("Task 'Calibration' running on core %d", xPortGetCoreID());
  for (;;) {
    this->glove_->calibrateSensors();
    // The task suspends itself in order to perform only a single calibration
    // per request.
    vTaskDelete(NULL);
  }
}

void TasksManager::stopRunningTask() {
  if (running_task_handler_ != nullptr) {
    log_i("Stopping running task.");
    vTaskDelete(running_task_handler_);
    running_task_handler_ = nullptr;
    log_i("Task stopped.");
    return;
  }
  log_i("Dropping stop command: no task was running.");
}

TasksManager::~TasksManager() {
  if (this->bleCommunicator != nullptr) {
    free(this->bleCommunicator);
  }
  if (this->tasksControllerCallback != nullptr) {
    free(this->tasksControllerCallback);
  }
}
