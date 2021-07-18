
/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x esta para arriba */
#include <WiFi.h>
#include <Adxl.h>
#include <Mpu.h>
#include <Ble.h>

const char* ssid = "BULL SHIP";//"Ensalada tomate y cebolla";
const char* password =  "eribri11";//"greta2012";
int event_count;

WiFiServer wifiServer(8080);
WiFiClient client;

using namespace std;
struct movement {
    string device_id;
    int event_num;
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
    event_count = 0;   
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

void sendEvent(){
  event_count = event_count + 1;
  Serial.print("send new event: ");Serial.println(event_count);
  movement mov = {
    .device_id = "12:4d:5g:f5:43",
    .event_num = event_count,
    .acc = 37.53467975321,
    .gyro = 37.9999999999
  };
  const int buffer_size= 1 + snprintf(NULL, 0,
                                    "{\"device_id\":%s, \"event_num\":%d, \"acc\":%.3f, \"gyro\":%.3f}\n", 
                                    mov.device_id.c_str(), mov.event_num, mov.acc, mov.gyro);
  Serial.print("  buffer_size: ");Serial.print(buffer_size);
  assert(buffer_size > 0);
  char package[buffer_size];
  int size_written = snprintf(package, buffer_size,
                                    "{\"device_id\":%s, \"event_num\":%d, \"acc\":%.3f, \"gyro\":%.3f}\n", 
                                    mov.device_id.c_str(), mov.event_num, mov.acc, mov.gyro);
  assert(size_written == buffer_size - 1);
  Serial.print("  size_written: ");Serial.print(size_written);
  Serial.print("  sending value via wifi: ");
  Serial.println(package);
  int bytes_sent = client.write(package, size_written); 
  Serial.print("bytes_sent: ");Serial.println(bytes_sent);
  delay(10); 
}


void loop() { //loop() runs on core 1
  Serial.println("waiting for new connection");
  client = wifiServer.available();
 
  if (client) {
    Serial.println("New client!!");
    while (client.connected()) {
      sendEvent();
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  delay(500);
}






