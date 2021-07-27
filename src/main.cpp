#include <Glove.h>
#include <Movement.h>
#include <Utils.h>
#include <WiFi.h>

#define SSID "Ensalada tomate y cebolla"  //"GAP Ocampo"   //"BULL SHIP";
#define PASSWORD "greta2012"              //"eribri11"   //"eribri11";
WiFiServer wifiServer(8080);
WiFiClient client;
int eventCount;
Glove glove;

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
  Serial.println("...............................................");
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
