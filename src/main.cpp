
/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x esta para arriba */
#include <Adxl.h>
#include <Mpu.h>
#include <Ble.h>

#define START_SENDING_MEASUREMENTS "start"
#define END_SENDING_MEASUREMENTS "end"
#define RIGHT_HAND_BLE_SERVICE "RightHandSmaortGlove"
#define FORMATTED_MEASUREMENT "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f"

Ble bluetooth;
char buf[10];

void setup() {
    Serial.begin(9600);   
    bluetooth.init(RIGHT_HAND_BLE_SERVICE);
    Serial.println("Open Glove_sla App anc connect using bluetooth");     
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
  waitAnyUserInput("Press any key to start sending msg");
  for(int i = 0.0; i < 100; i++){
    sprintf(buf, "mensaje %d .", i);
    bluetooth.indicate(buf);
    Serial.println("  sending value via bluetooth: "+ String(buf));
  }
  delay(100);
}


