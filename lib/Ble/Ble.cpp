#include <Ble.h>
bool deviceConnected;


Ble::Ble(){
    this->ack_count = 0;
}


void Ble::init(const std::string name){
    this->name = name;
    //create ble server
    BLEDevice::init(name);//"RightHandSmaortGlove");
    this->profileServer = BLEDevice::createServer();
    this->profileServer->setCallbacks(this);

    //creare service
    this->profileService = this->profileServer->createService(SERVICE_UUID);

    //create characteristic
    this->profileCharacteristic = this->profileService->createCharacteristic(CHARACTERISTIC_UUID_TX, 
                                                                                BLECharacteristic::PROPERTY_INDICATE | 
                                                                                BLECharacteristic::PROPERTY_WRITE    |
                                                                                BLECharacteristic::PROPERTY_READ 
                                                                            );
    this->profileCharacteristic->addDescriptor(new BLE2902());
    this->profileService->start();

    //Start advertising
    this->profileServer->getAdvertising()->start();

}

void Ble::onConnect(BLEServer *pServer) {
    deviceConnected = true;
    Serial.println("********* Device Connected :) ************");
            
}

void Ble::onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    Serial.println("********* Device Disconnected :( ************"); 
}

bool Ble::indicate(const std::string value){
    if(deviceConnected){
        this->profileCharacteristic->setValue(value);
        this->profileCharacteristic->indicate();
        Serial.println("send value: "+ String(value.c_str()));
        return true;
    }
    Serial.println("No client connected to notify....");
    return false;
}

void Ble::waitACK(){
    char expected_ack_response[ACK_SIZE];
    sprintf(expected_ack_response,"%dack",this->ack_count);
    Serial.print("Expected value: "); 
    Serial.println(expected_ack_response);
    std::string value = this->profileCharacteristic->getValue();
    while(value.empty() || strncmp(value.c_str(), expected_ack_response, ACK_SIZE) != 0){
        value = this->profileCharacteristic->getValue();
    }
    Serial.print("Device response: ");
    Serial.println(value.c_str());
    this->ack_count = this->ack_count + 1;
    if(this->ack_count == ACK_LIMIT){
        this->ack_count = 0;
    }
}

bool Ble::notify(const std::string value){
    if(deviceConnected){
        this->profileCharacteristic->setValue(value);
        this->profileCharacteristic->notify();
        Serial.println("send value: "+ String(value.c_str()));
        return true;
    }
    Serial.println("No client connected to notify....");
    return false;
}

Ble::~Ble(){}






