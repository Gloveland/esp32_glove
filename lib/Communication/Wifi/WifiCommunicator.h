#ifndef WIFI_COMMUNICATOR_H_
#define WIFI_COMMUNICATOR_H_

#include <WiFi.h>
#include <Mpu.h>

#include <utility>

/**
 * Class to handle the wifi communication between the device and the
 * application.
 */
class WifiCommunicator {
 public:
  explicit WifiCommunicator(std::string device_id) : device_id_(std::move(device_id)) {
    wifi_server_ = WiFiServer(kWifiPort);
  }

  void connectToNetwork();

  boolean awaitForClient();

  boolean clientIsConnected();

  void send(std::map<Finger::Value, ImuSensorMeasurement> measurements);

 private:
  const uint16_t kWifiPort = 8080;
  const std::string device_id_;
  long events_count_ = 0;
  WiFiServer wifi_server_;
  WiFiClient wifi_client_;
};
#endif // WIFI_COMMUNICATOR_H_