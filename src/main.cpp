
/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x esta para arriba */
#include <WiFi.h>
#include <Adxl.h>
#include <Mpu.h>
#include <Ble.h>

const char* ssid = "Ensalada tomate y cebolla";
const char* password =  "greta2012";

WiFiServer wifiServer(8080);
WiFiClient client;

#define PACKETSIZE 512

using namespace std;

enum request_type {
  DEVICE_INFO = 1, 
  MOVEMENT = 2, 
  UNKNOWN = 3,
};

request_type requestTypeFrom(const char * enumVal){
    if ( strcmp(enumVal, "DEVICE_INFO") == 0) return DEVICE_INFO;
    if ( strcmp(enumVal, "MOVEMENT ") == 0) return MOVEMENT ;
  return UNKNOWN ;
}

struct request {
  request_type type;
};

struct device_info {
  string id;
  float battery;
};

struct movement {
    float acc;
    float gyro;
};


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

void handleRequest(int requestType){
  switch (requestType){
    case DEVICE_INFO:{
      Serial.println("handling device info");
      device_info device = {
        .id = "ac:87:a3:0a:2d:1b",
        .battery =  37.5
      };
      const int buffer_size = 1 + snprintf(NULL, 0,"{\"id\":\"%s\",\"battery\":%.1f}\n", device.id.c_str(), device.battery);
      Serial.print("  buffer_size: ");Serial.print(buffer_size);
      assert(buffer_size > 0);

      char package[buffer_size];
      int size_written = snprintf(package, buffer_size,"{\"id\":\"%s\",\"battery\":%.1f}\n",device.id.c_str(), device.battery);
      assert(size_written == buffer_size - 1);
      Serial.print("  size_written: ");Serial.print(size_written);
      Serial.print("  sending value via wifi: ");
      Serial.println(package);
      int bytes_sent = client.write(package, size_written); 
      Serial.print("bytes_sent: ");Serial.println(bytes_sent);
      delay(10); 

      break;
    }
    case MOVEMENT:{
      Serial.println("handle movement");
      movement mov = {
        .acc = 37.53467975321,
        .gyro = 37.9999999999
      };
      const int buffer_size2 = 1 + snprintf(NULL, 0,"{\"acc\":%.3f,\"gyro\":%.3f}\n", mov.acc, mov.gyro);
      Serial.print("  buffer_size: ");Serial.print(buffer_size2);
      assert(buffer_size2 > 0);

      char package2[buffer_size2];
      int size_written2 = snprintf(package2, buffer_size2,"{\"acc\":%.3f,\"gyro\":%.3f}\n", mov.acc, mov.gyro);
      assert(size_written2 == buffer_size2 - 1);
      Serial.print("  size_written: ");Serial.print(size_written2);
      Serial.print("  sending value via wifi: ");
      Serial.println(package2);
      int bytes_sent = client.write(package2, size_written2); 
      Serial.print("bytes_sent: ");Serial.println(bytes_sent);
      delay(10); 

      break;
    }
    default:
      Serial.print("unhandle request type: ");Serial.println(requestType);
      break;
  }
}


void loop() { //loop() runs on core 1
  
  Serial.println("waiting for new connection");
  client = wifiServer.available();
 
  if (client) {
    Serial.println("New client!!");
    while (client.connected()) {
      while (client.available() > 0) {
        Serial.println("read client request");
        char c = client.read();
        int i = c - '0';
        Serial.println(i);
        handleRequest(i);
      }   
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  
  delay(500);
}






