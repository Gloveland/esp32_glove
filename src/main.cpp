
/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x esta para arriba */
#include <WiFi.h>
#include <Adxl.h>
#include <Mpu.h>
#include <Ble.h>

const char* ssid = "Ensalada tomate y cebolla";
const char* password =  "greta2012";

char buf[10];
WiFiServer wifiServer(8080);

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
}

void clearInput(){
  while (Serial.available()){ 
    Serial.read(); 
    /* clear the input buffer */
  }
}

void waitAnyUserInput(String msg){
  clearInput();
  Serial.println(msg);
  while (!Serial.available()){
    /* wait for a character */
  }  
  clearInput();
}

void loop() { //loop() runs on core 1
  Serial.println("waiting for new connection");
  WiFiClient client = wifiServer.available();
 
  if (client) {
    Serial.println("New client!!");
    while (client.connected()) {
      while (client.available() > 0) {
        char c = client.read();
        Serial.print(c);
      }
      delay(10);

      waitAnyUserInput("Press any key to start sending msg");
      client.write("hola soy jaz");
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  delay(500);
}


