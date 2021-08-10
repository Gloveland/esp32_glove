
#include "../lib/Communication/Ble/BleCommunicator.h"
#include "../lib/Communication/Wifi/WifiCommunicator.h"
#include "../lib/Communication/Ble/CharacteristicCallbacks.h"
#include <Glove.h>
#include <Utils.h>

#define RIGHT_HAND_BLE_SERVICE "RightHandSmartGlove"

#define TASK_DELAY_MS 500

[[noreturn]] void taskReadSensors(void *pvParameters);
TaskHandle_t readSenorsTaskHandler;

[[noreturn]] void taskWifiCommunication(void *pvParameters);
TaskHandle_t wifiCommunicationTaskHandler;

[[noreturn]] void taskBleCommunication(void *pvParameters);
TaskHandle_t bleCommunicationTaskHandler;

QueueHandle_t queue;
const int kQueueSize = 100;

Glove glove;
WifiCommunicator wifiCommunicator(Glove::getDeviceId());
BleCommunicator bleCommunicator;

void setUpGlove();
void setUpBleCommunicator();
void setUpWifiCommunicator();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  queue = xQueueCreate(kQueueSize, sizeof(GloveMeasurements));
  if (queue == nullptr) {
    Serial.println("Error creating the queue.");
  }

  setUpGlove();
//  setUpWifiCommunicator();
  setUpBleCommunicator();
}

void setUpGlove() {
  glove.init();
  waitAnyUserInput("Type key to start measuring movements...");
  xTaskCreatePinnedToCore(
      taskReadSensors,               // Task function
      "readSensors",         // Name of the task
      10000,             // Stack size of task
      NULL,              // Parameter of the task
      1,                    // Priority of the task
      &readSenorsTaskHandler,        // Task handle to keep track of created task
      0                      // Pin task to core 0
  );
}

void setUpBleCommunicator() {
  bleCommunicator.init(RIGHT_HAND_BLE_SERVICE, readSenorsTaskHandler);
  xTaskCreatePinnedToCore(
      taskBleCommunication,               // Task function
      "bleCommunication",         // Name of the task
      10000,                  // Stack size of task
      NULL,                   // Parameter of the task
      1,                         // Priority of the task
      &bleCommunicationTaskHandler,       // Task handle to keep track of created task
      1                           // Pin task to core 0
  );
}

void setUpWifiCommunicator() {
  wifiCommunicator.connectToNetwork();
  xTaskCreatePinnedToCore(
      taskWifiCommunication,          // Task function
      "wifiCommunication",            // Name of the task
      10000,                       // Stack size of task
      NULL,                        // Parameter of the task
      1,                           // Priority of the task
      &wifiCommunicationTaskHandler,  // Task handle to keep track of created task
      1
  );
}

void loop() { // loop() runs on core 1

}

[[noreturn]] void taskReadSensors(void *pvParameters) {
  Serial.println("Task 'read gloves' running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    for (int i = 0; i < kQueueSize; i++) {
      GloveMeasurements measurements = glove.readSensors();
      xQueueSend(queue, &measurements, portMAX_DELAY);
      delay(100);  //
    }
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
}

[[noreturn]] void taskBleCommunication(void *pvParameters) {
  Serial.println("Task 'Bluetooth transmission' running on core ");
  Serial.println(xPortGetCoreID());
  GloveMeasurements glove_measurements;
  for (;;) {  // loop in thread
    // bluetooth.notify(START_SENDING_MEASUREMENTS);
    for (int i = 0; i < kQueueSize; i++) {
      xQueueReceive(queue, &glove_measurements, portMAX_DELAY);
      bleCommunicator.send(glove_measurements);
    }
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
}

[[noreturn]] void taskWifiCommunication(void *pvParameters) {
  while (true) {
    wifiCommunicator.awaitForClient();
    for (int i = 0; i < kQueueSize && wifiCommunicator.clientIsConnected();
         i++) {
      GloveMeasurements glove_measurements;
      xQueueReceive(queue, &glove_measurements, portMAX_DELAY);
      wifiCommunicator.send(glove_measurements);
    }
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);  // v
    delay(500);
  }
}
