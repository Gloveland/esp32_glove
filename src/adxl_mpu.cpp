

/*esta prueba funciona mas o menos bien con un data rate de 2g. lee 1 cuando x esta para arriba */
#include <Adxl.h>
#include <Mpu.h>

#define I2C_SDA 33
#define I2C_SCL 32



Adxl adxl345;
Mpu mpu6050;

unsigned long start;


void setup() {
    Serial.begin(9600);               // Initiate serial communication for printing the results on the Serial monitor
    mpu6050.init(I2C_SDA, I2C_SCL);
    adxl345.init(I2C_SDA, I2C_SCL);
   
    Serial.println();
    Serial.println("Type key when all sensors are placed over an horizontal plane: X = 0g, Y = 0g, Z = +1g orientation"); 
    while (!Serial.available()){
      //wait for a character 
    }  
    // === Calibration === //
    adxl345.calibrate();
    mpu6050.calibrate();
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
    Serial.println("-------------------------------------------------------------------------------------");
    Serial.println("timestamp,mpuAccX,mpuAccY,mpuAccZ,mpuGyroX,mpuGyroY,mpuGyroZ,adxlAccY,adxlAcX,adxlAccZ");
    start = millis();

}

void loop() {
  if(millis() - start > 10000){
    Serial.println("-------------------------------------------------------------------------------------");
    while (!Serial.available()){
      //wait for a character 
    }
    while (Serial.available()){
      Serial.read();  // clear the input buffer
    }
    Serial.println("-------------------------------------------------------------------------------------");
    Serial.println("timestamp,mpuAccX,mpuAccY,mpuAccZ,mpuGyroX,mpuGyroY,mpuGyroZ,adxlAccY,adxlAcX,adxlAccZ");
    start = millis();
  }else{
    Serial.print(millis() - start);Serial.print(",");
    mpu6050.read();
    adxl345.read();
    Serial.println("");
  }
  delay(200);
}
