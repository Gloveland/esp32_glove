#include <Credentials.h>
#include "WifiCommunicator.h"

#define TASK_DELAY_MS 500;

/**
 * Connects the device to the wifi network specified by the {@var NETWORK_SSID}.
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

/**
 * Waits for a new client connection to occur.
 *
 * <p> If no connection happens, the wait will time out. This method should be
 * called iteratively until a connection happens.
 */
boolean WifiCommunicator::listenForClients() {
  Serial.println("[WifiCommunicator] Waiting for new connection.");
  wifi_client_ = this->wifi_server_.available();
  if (wifi_client_) {
    Serial.println("[WifiCommunicator] New client connected.");
    events_count_ = 0;
    return true;
  }
  return false;
}

/** Sends a GloveMeasurement via wifi. */
void WifiCommunicator::send(GloveMeasurements measurements) {
  JSONVar jsonHand = measurements.toJson();
  JSONVar jsonMovement;
  jsonMovement["kDeviceId"] = this->kDeviceId.c_str();
  jsonMovement["event_num"] = this->events_count_;
  jsonMovement["hand"] = jsonHand;
  String jsonString = JSON.stringify(jsonMovement);
  Serial.print("[WifiCommunicator] Sending value via wifi: ");
  Serial.println(jsonString);
  wifi_client_.write("\n");
  events_count_++;
  Serial.print("[WifiCommunicator] Event count: ");
  Serial.println(events_count_);
}

boolean WifiCommunicator::clientIsConnected() {
  return wifi_client_.connected();
}
