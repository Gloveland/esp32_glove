
#include <Glove.h>
#include <Utils.h>

#include <sstream>

#include "../lib/Communication/Ble/BleCommunicator.h"
#include "../lib/Communication/Wifi/WifiCommunicator.h"

#define RIGHT_HAND_BLE_SERVICE "RightHandSmartGlove"
#define TASK_DELAY_MS 500

extern const bool KDebug = true;

[[noreturn]] void taskDataCollection(void *pvParameters);
TaskHandle_t dataCollectionTaskHandler;

[[noreturn]] void taskWifiCommunication(void *pvParameters);
TaskHandle_t wifiCommunicationTaskHandler;

[[noreturn]] void taskBleCommunication(void *pvParameters);
TaskHandle_t bleCommunicationTaskHandler;

[[noreturn]] void taskInterpretation(void *pvParameters);
TaskHandle_t interpretationTaskHandler;

[[noreturn]] void taskCalibration(void *pvParameters);
TaskHandle_t calibrationTaskHandler;

QueueHandle_t queue;
const int kQueueSize = 100;

Glove glove;
WifiCommunicator wifiCommunicator(Glove::getDeviceId());
BleCommunicator bleCommunicator;
TasksControllerCallback *tasksControllerCallback;

void setUpGlove();
void setUpBleCommunicator();
void setUpWifiCommunicator();

void setup() {
  Serial.begin(9600);
  log_e("Test log level error -DCORE_DEBUG_LEVEL=1");
  log_w("Test log level warnign -DCORE_DEBUG_LEVEL=2");
  log_i("Test log level information -DCORE_DEBUG_LEVEL=3");
  log_d("Test log level debug -DCORE_DEBUG_LEVEL=4");
  log_v("Test log level verbose -DCORE_DEBUG_LEVEL=5");

  pinMode(LED_BUILTIN, OUTPUT);
  queue = xQueueCreate(kQueueSize, sizeof(GloveMeasurements));
  if (queue == nullptr) {
    Serial.println("Error creating the queue.");
  }

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
      taskCalibration,         // Task function
      "Calibration task",      // Name of the task
      10000,                      // Stack size of task
      NULL,                       // Parameter of the task
      1,                          // Priority of the task
      &calibrationTaskHandler, // Task handle to keep track of created task
      0                           // Pin task to core 0
      );
  vTaskSuspend(calibrationTaskHandler);
}

void setUpBleCommunicator() {
  tasksControllerCallback = new TasksControllerCallback(
      dataCollectionTaskHandler, interpretationTaskHandler,
      calibrationTaskHandler);
  bleCommunicator.init(RIGHT_HAND_BLE_SERVICE, tasksControllerCallback);
  xTaskCreatePinnedToCore(
      taskBleCommunication,          // Task function
      "bleCommunication",            // Name of the task
      10000,                         // Stack size of task
      NULL,                          // Parameter of the task
      1,                             // Priority of the task
      &bleCommunicationTaskHandler,  // Task handle to keep track of created
                                     // task
      1                              // Pin task to core 0
  );
}

void setUpWifiCommunicator() {
  wifiCommunicator.connectToNetwork();
  xTaskCreatePinnedToCore(
      taskWifiCommunication,          // Task function
      "wifiCommunication",            // Name of the task
      10000,                          // Stack size of task
      NULL,                           // Parameter of the task
      1,                              // Priority of the task
      &wifiCommunicationTaskHandler,  // Task handle to keep track of created
                                      // task
      1);
}

void loop() {}  // loop() runs on core 1

[[noreturn]] void taskDataCollection(void *pvParameters) {
  Serial.print("Task 'read gloves' running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    for (int i = 0; i < kQueueSize; i++) {
      GloveMeasurements measurements = glove.readSensors();
      xQueueSend(queue, &measurements, portMAX_DELAY);
      delay(100);
    }
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
}

[[noreturn]] void taskInterpretation(void *pvParameters) {
  Serial.print("Task 'Interpretation' running on core ");
  Serial.println(xPortGetCoreID());
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
  Serial.println("Task 'Calibration' running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    glove.calibrateSensors();
    // The task suspends itself in order to perform only a single calibration
    // per request.
    vTaskSuspend(calibrationTaskHandler);
  }
}

[[noreturn]] void taskBleCommunication(void *pvParameters) {
  Serial.print("Task 'Bluetooth transmission' running on core ");
  Serial.println(xPortGetCoreID());
  GloveMeasurements glove_measurements;
  for (;;) {
    for (int i = 0; i < kQueueSize; i++) {
      if (xQueueReceive(queue, &glove_measurements, portMAX_DELAY) == pdPASS) {
        bleCommunicator.sendMeasurements(glove_measurements);
      } else {
        Serial.println(
            "[taskBleCommunication] fail to receive item from queue ");
      }
    }
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
}

[[noreturn]] void taskWifiCommunication(void *pvParameters) {
  while (true) {
    wifiCommunicator.listenForClients();
    GloveMeasurements glove_measurements;
    for (int i = 0; i < kQueueSize && wifiCommunicator.clientIsConnected();
         i++) {
      if (xQueueReceive(queue, &glove_measurements, portMAX_DELAY) == pdPASS) {
        wifiCommunicator.send(glove_measurements);
      }
    }
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
    delay(500);
  }
}
