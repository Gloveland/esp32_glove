#include <Credentials.h>
#include "WifiCommunicator.h"
#include "../../Sensors/GloveMeasurements.h"

#define TASK_DELAY_MS 500;

/**
 * Connects the device to the wifi network specified by the
 * {@var NETWORK_SSID}:
 */
void WifiCommunicator::connectToNetwork() {
  WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);
  while (WiFiClass::status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to the WiFi network: ");
  Serial.print(WiFi.localIP());
  this->wifi_server_.begin();
}

//TODO(Darius): Rename method below, as this doesn't await, it times out and returns.
boolean WifiCommunicator::awaitForClient() {
  Serial.println("...............................................");
  Serial.println("waiting for new connection");
  wifi_client_ = this->wifi_server_.available();
  if (wifi_client_) {
    Serial.println("New client!!");
    events_count_ = 0;
    return true;
  }
  return false;
}

void WifiCommunicator::send(GloveMeasurements measurements) {
  JSONVar jsonHand = measurements.toJson();
  JSONVar jsonMovement;
  jsonMovement["kDeviceId"] = this->kDeviceId.c_str();
  jsonMovement["event_num"] = this->events_count_;
  jsonMovement["hand"] = jsonHand;
  Serial.print("  sending value via wifi: ");
  String jsonString = JSON.stringify(jsonMovement);
  Serial.println(jsonString);
  uint bytes_sent = wifi_client_.write(jsonString.c_str(), jsonString.length());
  Serial.print("bytes_sent: ");
  wifi_client_.write("\n");
  Serial.println(bytes_sent);
  events_count_++;
  Serial.print("Send new event: ");
  Serial.println(events_count_);
}

boolean WifiCommunicator::clientIsConnected() {
  return wifi_client_.connected();
}
