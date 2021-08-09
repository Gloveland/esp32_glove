
#include "../lib/Credentials/Credentials.h"
#include "../lib/Communication/Ble/Ble.h"
#include "../lib/Communication/Wifi/WifiCommunicator.h"
#include <Glove.h>
#include <Utils.h>
#include <WiFi.h>
#include <BLEServer.h>

#define START_SENDING_MEASUREMENTS "start"
#define END_SENDING_MEASUREMENTS "end"
#define RIGHT_HAND_BLE_SERVICE "RightHandSmartGlove"
Glove glove;
WifiCommunicator wifiCommunicator = WifiCommunicator(glove.getDeviceId());

int eventCount;
//Ble bluetooth;
//
//TaskHandle_t bluetoothTxTaskHandler;
//[[noreturn]] void taskBluetoothTransmission(void* pvParameters);
//bool sendMeasurementViaBluetooth(const FingerMeasurement measurement);

TaskHandle_t readGloveMovesTaskHandler;
[[noreturn]] void taskReadGloveMoves(void* pvParameters);

//void setUpWifiConnection();

//void setUpBleConnection();

void setUpGlove();

//void runWifiConnection();

//void runBleConnection();

void setup() {
  Serial.begin(9600);
  wifiCommunicator.connectToNetwork();
//  setUpWifiConnection();
//  setUpBleConnection();
  setUpGlove();
}

void setUpGlove() {
  glove.init();
  waitAnyUserInput("Type key to start measuring movements...");
}

void loop() { // loop() runs on core 1
  wifiCommunicator.awaitForClient();
  while (wifiCommunicator.clientIsConnected()) {
    auto measurements = glove.readSensors();
    Serial.print(">>>>"); Serial.println(measurements.at(Finger::Value::kPinky).toJson());
    wifiCommunicator.send(measurements);
  }

//  runWifiConnection();
//  runBleConnection();
  delay(500);
}

//void setUpWifiConnection() {
//    WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);
//    while (WiFi.status() != WL_CONNECTED) {
//      delay(1000);
//      Serial.println("Connecting to WiFi..");
//    }
//    Serial.print("Connected to the WiFi network: ");
//    Serial.print(WiFi.localIP());
//    wifiServer.begin();
//}


//void setUpBleConnection() {
//  BLEServerCallbacks *serverCallbacks = new ServerCallbacks();
//  BLECharacteristicCallbacks *characteristicCallbacks =
//      new CharacteristicCallbacks(readGloveMovesTaskHandler);
//  bluetooth.init(RIGHT_HAND_BLE_SERVICE, serverCallbacks,
//                 characteristicCallbacks);
//  Serial.println("Open Glove_sla App and connect using bluetooth.");
//  // create a task that will execute the transmission of the list
//  xTaskCreatePinnedToCore(
//      taskBluetoothTransmission,  // Task function
//      "bluetoothTxTask",          // Name of the task
//      10000,                      // Stack size of task
//      NULL,                       // Parameter of the task
//      1,                          // Priority of the task
//      &bluetoothTxTaskHandler,    // Task handle to keep track of created task
//      1                           // Pin task to core 0
//  );
//
//}

//void runBleConnection() {
//    Serial.println("...............................................");
//    Serial.println("Waiting for a new connection via BLE.");
//
//}

//void runWifiConnection() {
//    Serial.println("...............................................");
//    Serial.println("waiting for new connection");
//    client = wifiServer.available();
//    if (client) {
//      Serial.println("New client!!");
//      while (client.connected()) {
//        eventCount = eventCount + 1;
//        Serial.print("send new event: ");
//        Serial.println(eventCount);
//        Sensors mov = glove.readMovement(eventCount);
//        mov.send(client);
//      }
//      client.stop();
//      eventCount = 0;
//      Serial.println("Client disconnected");
//    }
//}

//[[noreturn]] void taskReadGloveMoves(void* pvParameters) {
//  Serial.println("Task 'read gloves' running on core ");
//  Serial.println(xPortGetCoreID());
//  for (;;) {  // loop in thread
//    for (int i = 0; i < queueSize; i++) {
//      FingerMeasurement thumbMeasurement = thumbSensor.read();
//      xQueueSend(queue, &thumbMeasurement, portMAX_DELAY);
//      // default rate is 8khz = 0.125 miliseconds
//      delay(100);  //
//    }
//    // Pause the task for 500ms otherwise watchdog will delete it causing
//    // memory error
//    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
//  }
//}
//
//[[noreturn]] void taskBluetoothTransmission(void* pvParameters) {
//  Serial.println("Task 'Bluetooth transmission' running on core ");
//  Serial.println(xPortGetCoreID());
//  FingerMeasurement fingerMeasurement{};
//  for (;;) {  // loop in thread
//    // bluetooth.notify(START_SENDING_MEASUREMENTS);
//    for (int i = 0; i < queueSize; i++) {
//      Serial.print("  Reading queue ");
//      Serial.println(i);
//      xQueueReceive(queue, &fingerMeasurement, portMAX_DELAY);
//      sendMeasurementViaBluetooth(fingerMeasurement);
//    }
//    // bluetooth.notify(END_SENDING_MEASUREMENTS);
//    // Pause the task for 500ms otherwise watchdog will delete it causing
//    // memory error
//    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);  // vTaskDelete( NULL );
//  }
//}
//
//bool sendMeasurementViaBluetooth(const FingerMeasurement measurement) {
//  // calculating the size of bluetooth payload buffer
//  const int buffer_size = 256;
//  char buffer[buffer_size];
//  int writtenChars = snprintf(
//      buffer, buffer_size, FORMATTED_MEASUREMENT, measurement.acc.X,
//      measurement.acc.Y, measurement.acc.Z, measurement.gyro.X,
//      measurement.gyro.Y, measurement.gyro.Z, measurement.inclination.roll,
//      measurement.inclination.pitch, measurement.inclination.yaw);
//
//  if (writtenChars < 0) {
//    return false;
//  }
//
//  bluetooth.notify(buffer);
//  return true;
//}