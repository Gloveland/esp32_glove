

/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x esta para arriba */
#include <Adxl.h>
#include <Mpu.h>
#include <Ble.h>

#define RIGHT_HAND_BLE_SERVICE "RightHandSmaortGlove"

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
int txValue = 0;
char txString[10];

unsigned long start;
void setupBleConnection();
void setupGlove();

void setup() {
    Serial.begin(9600);              
    setupBleConnection();
    setupGlove();
    start = millis();
}

void setupBleConnection(){
  bluetooth.init(RIGHT_HAND_BLE_SERVICE);
  Serial.println("Open Glove_sla App anc connect using bluetooth");
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
    
    while (Serial.available()){
      Serial.read();  // clear the input buffer
    }

    Serial.println("Type key to start mesuring acceleration..."); 
    while (!Serial.available()){
      //wait for a character 
    }  
    while (Serial.available()){
      Serial.read();  // clear the input buffer
    }
}


void loop() {
  if(millis() - start > 10000){
    Serial.println("---------------------------------------END----------------------------------------------");

    txValue = random(-10,20);
    //conversion
    //dtostrf(txValue, 1, 2, txString);
    //bluetooth.indicate(txString);
    sprintf(txString, "Random= %d",txValue);
    //bluetooth.notify(txString);
    
    //bluetooth.notifyWithAck(txString);
    

    Serial.println("Type key to start mesuring acceleration..."); 
    while (!Serial.available()){
      //wait for a character 
    }
    while (Serial.available()){
      Serial.read();  // clear the input buffer
    }
    Serial.println("-------------------------------------------------------------------------------------");
    start = millis();
  }else{
    Serial.print(millis() - start);Serial.print(",");
    /*
    Serial.print("P:");
    pinkySensor.read();
    Serial.print("     R:");
    ringSensor.read();
    Serial.print("     M:");
    middleSensor.read();
    Serial.print("     I:");
    indexSensor.read(); 
    */
    Serial.print("    T:");
    Mesure mesure = thumpSensor.read();
    Serial.print("   aX: ");
    Serial.print(mesure.acc.X);
    Serial.print("   aY: ");
    Serial.print(mesure.acc.Y);
    Serial.print("   aZ: ");
    Serial.print(mesure.acc.Z);Serial.print(",");

    Serial.print("   gX: ");
    Serial.print(mesure.gyro.X);Serial.print(",");
    Serial.print("   gY: ");
    Serial.print(mesure.gyro.Y);Serial.print(",");
    Serial.print("   gZ: ");
    Serial.print(mesure.gyro.Z);Serial.print(",");
    Serial.print("   degrees/seg");

    Serial.print("  roll:"); Serial.print(mesure.inclination.roll);
    Serial.print("  pitch:");Serial.print(mesure.inclination.pitch);
    Serial.print("  yaw:"); Serial.print(mesure.inclination.yaw);
    Serial.println("");
   
  }
  delay(200);
}
