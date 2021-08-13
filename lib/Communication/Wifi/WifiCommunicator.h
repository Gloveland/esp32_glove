#ifndef WIFI_COMMUNICATOR_H_
#define WIFI_COMMUNICATOR_H_

#include <WiFi.h>
#include <Mpu.h>

#include <utility>
#include "../../Sensors/GloveMeasurements.h"

/**
 * Class to handle the wifi communication between the device and the
 * application.
 */
class WifiCommunicator {
 public:
  explicit WifiCommunicator(std::string device_id) :
      kDeviceId(std::move(device_id)) {
    wifi_server_ = WiFiServer(kWifiPort);
  }

  void connectToNetwork();

  boolean listenForClients();

  boolean clientIsConnected();

  void send(GloveMeasurements measurements);

 private:
  const uint16_t kWifiPort = 8080;
  const std::string kDeviceId;
  long events_count_ = 0;
  WiFiServer wifi_server_;
  WiFiClient wifi_client_;
};
#endif // WIFI_COMMUNICATOR_H_
