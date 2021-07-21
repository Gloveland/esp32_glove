
/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x
 * esta para arriba */
#include <WiFi.h>
//#include <Adxl.h>
#include <Mpu.h>
//#include <Ble.h>
#include <Model.h>

#define FORMATTED_JSON_MEASUREMENT                                            \
  "{\"device_id\":\"%s\", \"event_num\":%d, \"hand\": { \"thump\": { "        \
  "\"acc\": { \"x\":%.3f, \"y\":%.3f, \"z\":%.3f }, \"gyro\": { \"x\":%.3f, " \
  "\"y\":%.3f, \"z\":%.3f }, \"inclination\": { \"roll\":%.3f, "              \
  "\"pitch\":%.3f, \"yaw\":%.3f } } } }\n"

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

const char *ssid = "Ensalada tomate y cebolla";  //"BULL SHIP";
const char *password = "greta2012";              //"eribri11";
int event_count;
// Mpu pinkySensor;
// Mpu ringSensor;
// Mpu middleSensor;
// Mpu indexSensor;
Mpu thumpSensor;
WiFiServer wifiServer(8080);
WiFiClient client;

void setupGlove();

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  wifiServer.begin();

  setupGlove();
  event_count = 0;
}

void setupGlove() {
  // pinkySensor.init(I2C_SDA_PINKY, I2C_SCL_PINKY);
  // ringSensor.init(I2C_SDA_RING, I2C_SCL_RING);
  // middleSensor.init(I2C_SDA_MIDDLE, I2C_SCL_MIDDLE);
  // indexSensor.init(I2C_SDA_INDEX, I2C_SCL_INDEX);
  thumpSensor.init(I2C_SDA_THUMB, I2C_SCL_THUMB);

  Serial.println();
  Serial.println(
      "Type key when all sensors are placed over an horizontal plane: X = 0g, "
      "Y = 0g, Z = +1g orientation");
  while (!Serial.available()) {
    // wait for a character
  }
  // === Calibration === //
  // pinkySensor.calibrate();
  // ringSensor.calibrate();
  // middleSensor.calibrate();
  // indexSensor.calibrate();
  thumpSensor.calibrate();

  while (Serial.available()) {
    Serial.read();  // clear the input buffer
  }
  Serial.println("Type key to start mesuring acceleration...");
  while (!Serial.available()) {
    // wait for a character
  }
  while (Serial.available()) {
    Serial.read();  // clear the input buffer
  }
}

void clearInput() {
  while (Serial.available()) {
    Serial.read();
    /* clear the input buffer */
  }
}

void waitAnyUserInput(String msg) {
  clearInput();
  Serial.println(msg);
  while (!Serial.available()) {
    /* wait for a character */
  }
  clearInput();
}

void sendEvent() {
  event_count = event_count + 1;
  Serial.print("send new event: ");
  Serial.println(event_count);
  /*

  Acceleration acc = {
    .x = 0.1256,
    .y = 0.345629,
    .z = 0.569999,
  };
  Gyro gyro = {
    .x = 1.1256,
    .y = 2.345629,
    .z = 3.569999,
  };
  Inclination inclination = {
    .roll = 180.45,
    .pitch = 14.34,
    .yaw = 360.56899,
  };
  Finger thump = {
    .acc = acc,
    .gyro = gyro,
    .inclination = inclination,
  };
  */
  Finger thump = thumpSensor.read();
  Hand hand = {
      .thump = thump,
  };
  Movement mov = {
      .device_id = "12:4d:5g:f5:43",
      .event_num = event_count,
      .hand = hand,
  };
  const int buffer_size =
      1 + snprintf(NULL, 0, FORMATTED_JSON_MEASUREMENT, mov.device_id.c_str(),
                   mov.event_num, mov.hand.thump.acc.x, mov.hand.thump.acc.y,
                   mov.hand.thump.acc.z, mov.hand.thump.gyro.x,
                   mov.hand.thump.gyro.y, mov.hand.thump.gyro.z,
                   mov.hand.thump.inclination.roll,
                   mov.hand.thump.inclination.pitch,
                   mov.hand.thump.inclination.yaw);
  Serial.print("  buffer_size: ");
  Serial.print(buffer_size);
  assert(buffer_size > 0);
  char package[buffer_size];
  int size_written = snprintf(
      package, buffer_size, FORMATTED_JSON_MEASUREMENT, mov.device_id.c_str(),
      mov.event_num, mov.hand.thump.acc.x, mov.hand.thump.acc.y,
      mov.hand.thump.acc.z, mov.hand.thump.gyro.x, mov.hand.thump.gyro.y,
      mov.hand.thump.gyro.z, mov.hand.thump.inclination.roll,
      mov.hand.thump.inclination.pitch, mov.hand.thump.inclination.yaw);
  assert(size_written == buffer_size - 1);
  Serial.print("  size_written: ");
  Serial.print(size_written);
  Serial.print("  sending value via wifi: ");
  Serial.println(package);
  int bytes_sent = client.write(package, size_written);
  Serial.print("bytes_sent: ");
  Serial.println(bytes_sent);
  delay(10);
}

void loop() {  // loop() runs on core 1
  Serial.println("waiting for new connection");
  client = wifiServer.available();

  if (client) {
    Serial.println("New client!!");
    while (client.connected()) {
      sendEvent();
    }
    client.stop();
    event_count = 0;
    Serial.println("Client disconnected");
  }
  delay(500);
}
