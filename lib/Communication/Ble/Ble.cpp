//#include <Ble.h>
//bool deviceConnected;
//
//Ble::Ble() { this->ack_count = this->ACK_INIT; }
//
//void Ble::init(const std::string name) {
//  this->name = name;
//
//  BLEDevice::init(name);
//  this->profileServer = BLEDevice::createServer();
//  BLEServerCallbacks* serverCallbacks = new ServerCallbacks();
//  this->profileServer->setCallbacks(serverCallbacks);
//
//  this->profileService = this->profileServer->createService(SERVICE_UUID);
//  this->profileCharacteristic = this->profileService->createCharacteristic(
//      CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_INDICATE |
//                                  BLECharacteristic::PROPERTY_NOTIFY |
//                                  BLECharacteristic::PROPERTY_WRITE |
//                                  BLECharacteristic::PROPERTY_READ);
//  this->profileCharacteristic->addDescriptor(new BLE2902());
//  this->profileService->start();
//
//  this->profileServer->getAdvertising()->start();
//}
//
//void Ble::onConnect(BLEServer *pServer) {
//    deviceConnected = true;
//    Serial.println("********* Device Connected :) ************");
//
//}
//
//void Ble::onDisconnect(BLEServer *pServer) {
//    deviceConnected = false;
//    Serial.println("********* Device Disconnected :( ************");
//}
//
//void Ble::indicate(const std::string value){
//    this->profileCharacteristic->setValue(value);
//    this->profileCharacteristic->indicate();
//    Serial.println("send value: "+ String(value.c_str()));
//}
//
//void Ble::notify(const std::string value){
//    this->profileCharacteristic->setValue(value);
//    this->profileCharacteristic->notify();
//    Serial.println("send value: "+ String(value.c_str()));
//}
//
//void Ble::notifyWithAck(const std::string value) {
//  this->notify(value);
//  char expected_ack_response[this->ACK_SIZE];
//  sprintf(expected_ack_response, "%dack", this->ack_count);
//  Serial.print("Expected value: ");
//  Serial.println(expected_ack_response);
//  std::string response = this->profileCharacteristic->getValue();
//  while (response.empty() ||
//         strncmp(response.c_str(), expected_ack_response, ACK_SIZE) != 0) {
//    if (!deviceConnected) {
//      Serial.println("No client connected yet....");
//      this->profileServer->getAdvertising()->start();
//      this->ack_count = this->ACK_INIT;
//    }
//    this->notify(value);
//    response = this->profileCharacteristic->getValue();
//    Serial.print("App response: ");
//    Serial.println(response.c_str());
//    // Pause the task for 100ms otherwise watchdog will delete it cousing memory
//    // error
//    vTaskDelay(100 / portTICK_PERIOD_MS);
//  }
//  this->ack_count = this->ack_count + 1;
//  if (this->ack_count == this->ACK_LIMIT) {
//    this->ack_count = this->ACK_INIT;
//  }
//}
//
//Ble::~Ble() {}
