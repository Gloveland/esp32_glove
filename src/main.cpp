//#include <Adxl.h>
//#include <Ble.h>
#include <Glove.h>
#include <Movement.h>
#include <WiFi.h>

using namespace std;
#define SSID "GAP Ocampo"    //"Ensalada tomate y cebolla"  //"BULL SHIP";
#define PASSWORD "eribri11"  //"greta2012"  //"eribri11";
WiFiServer wifiServer(8080);
WiFiClient client;
int eventCount;
Glove glove;

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

void setup() {
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to the WiFi network: ");
  Serial.print(WiFi.localIP());
  wifiServer.begin();
  glove.init();
  eventCount = 0;
  waitAnyUserInput("Type key to start mesuring movements...");
}

void loop() {  // loop() runs on core 1
  Serial.println("waiting for new connection");
  client = wifiServer.available();
  if (client) {
    Serial.println("New client!!");
    while (client.connected()) {
      eventCount = eventCount + 1;
      Serial.print("send new event: ");
      Serial.println(eventCount);
      Movement mov = glove.readMovement(eventCount);
      mov.send(client);
    }
    client.stop();
    eventCount = 0;
    Serial.println("Client disconnected");
  }
  delay(500);
}
