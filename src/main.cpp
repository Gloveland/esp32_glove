
/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x
 * esta para arriba */
#include <Adxl.h>
#include <Ble.h>
#include <Mpu.h>

#define START_SENDING_MEASUREMENTS "start"
#define END_SENDING_MEASUREMENTS "end"
#define RIGHT_HAND_BLE_SERVICE "RightHandSmartGlove"
#define FORMATTED_MEASUREMENT "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f"

#define I2C_SCL_PINKY 27 /* gris */
#define I2C_SDA_PINKY 26 /* violeta */

#define I2C_SCL_RING 33 /* azul */
#define I2C_SDA_RING 32 /* verde */

#define I2C_SCL_MIDDLE 25 /* amarillo */
#define I2C_SDA_MIDDLE 23 /* naranja */

#define I2C_SCL_INDEX 19 /* azul */
#define I2C_SDA_INDEX 18 /* verde */

#define I2C_SCL_THUMB 22 /* amarillo */
#define I2C_SDA_THUMB 21 /* naranja */

/* Delay between each loop iteration within a task in milliseconds. */
#define TASK_DELAY_MS 500

// Mpu pinkySensor;
// Mpu ringSensor;
// Mpu middleSensor;
// Mpu indexSensor;
Mpu thumbSensor;
Ble bluetooth;

void setupBleConnection();
void setupGlove();

QueueHandle_t queue;
const int queueSize = 100;

TaskHandle_t bluetoothTxTaskHandler;
[[noreturn]] void taskBluetoothTransmission(void* pvParameters);
bool sendMeasurementViaBluetooth(const FingerMeasurement measurement);

TaskHandle_t readGloveMovesTaskHandler;
[[noreturn]] void taskReadGloveMoves(void* pvParameters);

void setup() {
  Serial.begin(9600);
  queue = xQueueCreate(queueSize, sizeof(FingerMeasurement));
  if (queue == NULL) {
    Serial.println("Error creating the queue");
  }

  setupGlove();
  setupBleConnection();
}

void setupGlove() {
  // pinkySensor.init(I2C_SDA_PINKY, I2C_SCL_PINKY);
  // ringSensor.init(I2C_SDA_RING, I2C_SCL_RING);
  // middleSensor.init(I2C_SDA_MIDDLE, I2C_SCL_MIDDLE);
  // indexSensor.init(I2C_SDA_INDEX, I2C_SCL_INDEX);
  thumbSensor.init(I2C_SDA_THUMB, I2C_SCL_THUMB);

  // === Calibration === //
  // pinkySensor.calibrate();
  // ringSensor.calibrate();
  // middleSensor.calibrate();
  // indexSensor.calibrate();
  thumbSensor.calibrate();

  // create a task that will execute the transmission of the list
  xTaskCreatePinnedToCore(
      taskReadGloveMoves,          // Task function
      "readGloveMoves",            // Name of the task
      10000,                       // Stack size of task
      NULL,                        // Parameter of the task
      1,                           // Priority of the task
      &readGloveMovesTaskHandler,  // Task handle to keep track of created task
      0                            // Pin task to core 0
  );
}

/**
 * @brief Setup the BLE connection.
 *
 * Internally runs the ::taskBluetoothTransmission task.
 */
void setupBleConnection() {
  bluetooth.init(RIGHT_HAND_BLE_SERVICE);
  Serial.println("Open Glove_sla App and connect using bluetooth.");

  // create a task that will execute the transmission of the list
  xTaskCreatePinnedToCore(
      taskBluetoothTransmission,  // Task function
      "bluetoothTxTask",          // Name of the task
      10000,                      // Stack size of task
      NULL,                       // Parameter of the task
      1,                          // Priority of the task
      &bluetoothTxTaskHandler,    // Task handle to keep track of created task
      1                           // Pin task to core 0
  );
}

/**
 * @brief Clears the input buffer.
 *
 * @note Pauses the task for #TASK_DELAY_MS in order
 */
void clearInputBuffer() {
  while (Serial.available()) {
    Serial.read();
    // Pause the task for 500ms otherwise watchdog will delete it causing
    // memory error
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
}

/**
 * @brief Waits for any input written by the user.
 *
 * @param msg the user input.
 */
void waitAnyUserInput(String msg) {
  clearInputBuffer();
  Serial.println(msg);
  while (!Serial.available()) {
    /* wait for a character */
    // Pause the task for 500ms otherwise watchdog will delete it causing
    // memory error
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
  clearInputBuffer();
}

void loop() {  // loop() runs on core 1
               // empty because loop is in tasks
}

[[noreturn]] void taskReadGloveMoves(void* pvParameters) {
  Serial.println("Task 'read gloves' running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {  // loop in thread
    for (int i = 0; i < queueSize; i++) {
      FingerMeasurement thumbMeasurement = thumbSensor.read();
      xQueueSend(queue, &thumbMeasurement, portMAX_DELAY);
      // default rate is 8khz = 0.125 miliseconds
      delay(100);  //
    }
    // Pause the task for 500ms otherwise watchdog will delete it causing
    // memory error
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);
  }
}

[[noreturn]] void taskBluetoothTransmission(void* pvParameters) {
  Serial.println("Task 'Bluetooth transmission' running on core ");
  Serial.println(xPortGetCoreID());
  FingerMeasurement fingerMeasurement{};
  for (;;) {  // loop in thread
    // bluetooth.notify(START_SENDING_MEASUREMENTS);
    for (int i = 0; i < queueSize; i++) {
      Serial.print("  Reading queue ");
      Serial.println(i);
      xQueueReceive(queue, &fingerMeasurement, portMAX_DELAY);
      sendMeasurementViaBluetooth(fingerMeasurement);
    }
    // bluetooth.notify(END_SENDING_MEASUREMENTS);
    // Pause the task for 500ms otherwise watchdog will delete it causing
    // memory error
    vTaskDelay(TASK_DELAY_MS / portTICK_PERIOD_MS);  // vTaskDelete( NULL );
  }
}

bool sendMeasurementViaBluetooth(const FingerMeasurement measurement) {
  // calculating the size of bluetooth payload buffer
  const int buffer_size = 256;
  char buffer[buffer_size];
  int writtenChars = snprintf(
      buffer, buffer_size, FORMATTED_MEASUREMENT, measurement.acc.X,
      measurement.acc.Y, measurement.acc.Z, measurement.gyro.X,
      measurement.gyro.Y, measurement.gyro.Z, measurement.inclination.roll,
      measurement.inclination.pitch, measurement.inclination.yaw);

  if (writtenChars < 0) {
    return false;
  }

  bluetooth.notify(buffer);
  return true;
}
