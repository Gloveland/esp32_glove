
#include <Glove.h>
#include <Utils.h>
#include <esp_task_wdt.h>

#include <sstream>

#include "../lib/Communication/Ble/BleCommunicator.h"
#include "../lib/Glove/Counter.h"

#define RIGHT_HAND_BLE_SERVICE "RightHandSmartGlove"
#define TASK_DELAY_MS 500

[[noreturn]] void taskDataCollection(void *pvParameters);
TaskHandle_t dataCollectionTaskHandler;

[[noreturn]] void taskWifiCommunication(void *pvParameters);
TaskHandle_t wifiCommunicationTaskHandler;

[[noreturn]] void taskInterpretation(void *pvParameters);
TaskHandle_t interpretationTaskHandler;

[[noreturn]] void taskCalibration(void *pvParameters);
TaskHandle_t calibrationTaskHandler;

Glove glove;
Counter *counter;
BleCommunicator bleCommunicator;
TasksControllerCallback *tasksControllerCallback;

void setUpGlove();
void setUpBleCommunicator();
void setUpWifiCommunicator();

void setup() {
  Serial.begin(9600);
  counter = new Counter();
  log_e("Test log level error -DCORE_DEBUG_LEVEL=1");
  log_w("Test log level warnign -DCORE_DEBUG_LEVEL=2");
  log_i("Test log level information -DCORE_DEBUG_LEVEL=3");
  log_d("Test log level debug -DCORE_DEBUG_LEVEL=4");
  log_v("Test log level verbose -DCORE_DEBUG_LEVEL=5");
  pinMode(LED_BUILTIN, OUTPUT);
  esp_task_wdt_init(100, false);

  setUpGlove();

  // In order to switch between communication modes, comment and uncomment
  // the lines below:
  //  setUpWifiCommunicator();
  setUpBleCommunicator();
}

void setUpGlove() {
  glove.init();
  xTaskCreatePinnedToCore(
      taskDataCollection,          // Task function
      "readSensors",               // Name of the task
      10000,                       // Stack size of task
      NULL,                        // Parameter of the task
      1,                           // Priority of the task
      &dataCollectionTaskHandler,  // Task handle to keep track of created task
      0                            // Pin task to core 0
  );
  vTaskSuspend(dataCollectionTaskHandler);
  xTaskCreatePinnedToCore(
      taskInterpretation,          // Task function
      "Interpretation task",       // Name of the task
      10000,                       // Stack size of task
      NULL,                        // Parameter of the task
      1,                           // Priority of the task
      &interpretationTaskHandler,  // Task handle to keep track of created task
      0                            // Pin task to core 0
  );
  vTaskSuspend(interpretationTaskHandler);
  xTaskCreatePinnedToCore(
      taskCalibration,          // Task function
      "Calibration task",       // Name of the task
      10000,                    // Stack size of task
      NULL,                     // Parameter of the task
      1,                        // Priority of the task
      &calibrationTaskHandler,  // Task handle to keep track of created task
      0                         // Pin task to core 0
  );
  vTaskSuspend(calibrationTaskHandler);
}

void setUpBleCommunicator() {
  tasksControllerCallback = new TasksControllerCallback(
      dataCollectionTaskHandler, interpretationTaskHandler,
      calibrationTaskHandler, counter);
  bleCommunicator.init(RIGHT_HAND_BLE_SERVICE, tasksControllerCallback);
}

void loop() {}  // loop() runs on core 1

[[noreturn]] void taskDataCollection(void *pvParameters) {
  log_i("Task 'read gloves' running on core %d", xPortGetCoreID());
  float elapsedTime;
  for (;;) {
    elapsedTime = counter->getAndUpdateElapsedTimeSinceLastMeasurementMs();
    GloveMeasurements measurements = glove.readSensors();
    log_i("frequency: %.3f hz",
          1.0 / (elapsedTime / 1000.0));  // Divide by 1000 to get seconds
    int count = counter->getAndUpdateCounter();
    bleCommunicator.sendMeasurements(
        measurements.toPackage(count, elapsedTime));
    log_i("Counter: %d", count);
    log_i("Elapsed time: %f", elapsedTime);
  }
}

[[noreturn]] void taskInterpretation(void *pvParameters) {
  log_i("Task 'Interpretation' running on core %d", xPortGetCoreID());
  for (;;) {
    for (int i = 0; i < 5; i++) {
      // TODO(https://git.io/Ju6I7): Implement translation task.
      std::stringstream mock_interpretation_stream;
      mock_interpretation_stream << "Interpretation " << i;
      bleCommunicator.sendInterpretation(mock_interpretation_stream.str());
      delay(100);
    }
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
}

[[noreturn]] void taskCalibration(void *pvParameters) {
  log_i("Task 'Calibration' running on core %d", xPortGetCoreID());
  for (;;) {
    glove.calibrateSensors();
    // The task suspends itself in order to perform only a single calibration
    // per request.
    vTaskSuspend(calibrationTaskHandler);
  }
}
