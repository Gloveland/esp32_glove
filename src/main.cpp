
/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x esta para arriba */
#include <Adxl.h>
#include <Mpu.h>
#include <Ble.h>

#define START_SENDING_MEASUREMENTS "start"
#define END_SENDING_MEASUREMENTS "end"
#define RIGHT_HAND_BLE_SERVICE "RightHandSmaortGlove"
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

//Mpu pinkySensor;
//Mpu ringSensor;
//Mpu middleSensor;
//Mpu indexSensor;
Mpu thumpSensor;
Ble bluetooth;

void setupBleConnection();
void setupGlove();

QueueHandle_t queue;
int queueSize = 100;
TaskHandle_t bluetoothTxTaskHandler;
void taskBluetoothTransmission( void * pvParameters );
bool sendMesurementViaBluetooth(const FingerMesurements mesure);

TaskHandle_t readGloveMovesTaskHandler;
void taskReadGloveMoves( void * pvParameters );



void setup() {
    Serial.begin(9600); 
    queue = xQueueCreate( queueSize, sizeof(FingerMesurements));
    if(queue == NULL){
      Serial.println("Error creating the queue");
    }

    setupGlove();              
    setupBleConnection();
}

void setupGlove(){
    //pinkySensor.init(I2C_SDA_PINKY, I2C_SCL_PINKY);
    //ringSensor.init(I2C_SDA_RING, I2C_SCL_RING);
    //middleSensor.init(I2C_SDA_MIDDLE, I2C_SCL_MIDDLE);
    //indexSensor.init(I2C_SDA_INDEX, I2C_SCL_INDEX);
    thumpSensor.init(I2C_SDA_THUMB, I2C_SCL_THUMB);
    
    Serial.println();
    Serial.println("Type key when all sensors are placed over an horizontal plane: X = 0g, Y = 0g, Z = +1g orientation"); 
    while (!Serial.available()){
      //wait for a character 
    }  
    // === Calibration === //
    //pinkySensor.calibrate();
    //ringSensor.calibrate();
    //middleSensor.calibrate();
    //indexSensor.calibrate();
    thumpSensor.calibrate();

    //create a task that will be executed the transmission of the list
    xTaskCreatePinnedToCore(
        taskReadGloveMoves,       /* Task function. */
        "readGloveMoves",         /* name of task. */
        10000,                    /* Stack size of task */
        NULL,                     /* parameter of the task */
        1,                        /* priority of the task */
        &readGloveMovesTaskHandler,  /* Task handle to keep track of created task */
        0                         /* pin task to core 0 */
    ); 
}

void setupBleConnection(){
    bluetooth.init(RIGHT_HAND_BLE_SERVICE);
    Serial.println("Open Glove_sla App anc connect using bluetooth");
  
    //create a task that will be executed the transmission of the list
    xTaskCreatePinnedToCore(
        taskBluetoothTransmission,/* Task function. */
        "bluetoothTxTask",        /* name of task. */
        10000,                    /* Stack size of task */
        NULL,                     /* parameter of the task */
        1,                        /* priority of the task */
        &bluetoothTxTaskHandler,  /* Task handle to keep track of created task */
        1                         /* pin task to core 0 */
    ); 
}

void clearInput(){
  while (Serial.available()){ 
    Serial.read(); 
    /* clear the input buffer */
    // Pause the task for 500ms otherwise watchdog will delete it cousing memory error
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void waitAnyUserInput(String msg){
  clearInput();
  Serial.println(msg);
  while (!Serial.available()){
    /* wait for a character */
    // Pause the task for 500ms otherwise watchdog will delete it cousing memory error
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }  
  clearInput();
}

void loop() { //loop() runs on core 1
  // empty because loop is in tasks
}

void taskReadGloveMoves( void * pvParameters ){
  for(;;){ //loop in thread
    Serial.println("Task2 running on core ");
    Serial.println(xPortGetCoreID());
    waitAnyUserInput("Type key to start mesuring..."); 
    Serial.println("---------------------------------------END----------------------------------------------");
    for( int i = 0; i < queueSize; i++){
      FingerMesurements thumpMesure = thumpSensor.read();
      xQueueSend(queue, &thumpMesure, portMAX_DELAY);
      //default rate is 8khz = 0.125 miliseconds
      delay(100); //
    }
    Serial.println("---------------------------------------END----------------------------------------------");
    // Pause the task for 500ms otherwise watchdog will delete it cousing memory error
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void taskBluetoothTransmission( void * pvParameters ){
  Serial.println("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  FingerMesurements m;
  for(;;){ //loop in thread
    bluetooth.notifyWithAck(START_SENDING_MEASUREMENTS);
    for(int i = 0; i < queueSize; i++ ){
        Serial.print("  Reading queue ");Serial.println(i);
        xQueueReceive(queue,  &m, portMAX_DELAY);
        sendMesurementViaBluetooth(m);
    }
    bluetooth.notifyWithAck(END_SENDING_MEASUREMENTS);
    // Pause the task for 500ms otherwise watchdog will delete it cousing memory error
    vTaskDelay(500 / portTICK_PERIOD_MS); //vTaskDelete( NULL );
  }
}

bool sendMesurementViaBluetooth(const FingerMesurements mesure){
  //calculating the size of bluetooth payload buffer
  const int buffer_size = 1 + snprintf(NULL, 0, FORMATTED_MEASUREMENT, 
    mesure.acc.X, mesure.acc.Y, mesure.acc.Z, 
    mesure.gyro.X, mesure.gyro.Y, mesure.gyro.Z,
    mesure.inclination.roll,mesure.inclination.pitch,mesure.inclination.yaw
  );
  //Serial.print("  buffer_size: ");Serial.print(buffer_size);
  assert(buffer_size > 0);
  //bluetooth payload buffer
  char buf[buffer_size];
  int size_written =  sprintf(buf, FORMATTED_MEASUREMENT, 
    mesure.acc.X, mesure.acc.Y, mesure.acc.Z, 
    mesure.gyro.X, mesure.gyro.Y, mesure.gyro.Z,
    mesure.inclination.roll,mesure.inclination.pitch,mesure.inclination.yaw
  );
  assert(size_written == buffer_size - 1);
  //Serial.print("  size_written: ");Serial.print(size_written);
  Serial.println("  sending value via bluetooth: "+ String(buf));
  bluetooth.notifyWithAck(buf);
  return true;
}
