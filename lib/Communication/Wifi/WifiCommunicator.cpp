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
    log_i("Connecting to WiFi..");
  }
  log_i("Connected to the WiFi network: %s ", WiFi.localIP());
  this->wifi_server_.begin();
}

/**
 * Waits for a new client connection to occur.
 *
 * <p> If no connection happens, the wait will time out. This method should be
 * called iteratively until a connection happens.
 */
boolean WifiCommunicator::listenForClients() {
  log_i("Waiting for new connection.");
  wifi_client_ = this->wifi_server_.available();
  if (wifi_client_) {
    log_i("New client connected.");
    events_count_ = 0;
    return true;
  }
  return false;
}

/** Sends a GloveMeasurement via wifi. */
void WifiCommunicator::send(GloveMeasurements measurements) {
  measurements.toPackage(this->events_count_, this->glove_measurement_buffer_, this->kMtu);
  log_i("Sending value via wifi: ");
  wifi_client_.write(this->glove_measurement_buffer_);
  log_i("Event count: %d", events_count_);
  this->events_count_ += 1;
}

boolean WifiCommunicator::clientIsConnected() {
  return wifi_client_.connected();
}
