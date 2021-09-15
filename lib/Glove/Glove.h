#ifndef GLOVE_H
#define GLOVE_H

#include <GloveMeasurements.h>
#include <Mpu.h>
#include <esp_task.h>

#include <map>

typedef std::map<Finger::Value, Mpu> GloveSensors;

class Glove {
 public:
  void init();
  GloveMeasurements readSensors();
  static std::string getDeviceId();
  ~Glove();

 private:
  static const int kchip_id_str_size;
  static const std::string kchip_id_format;
  static const int kchip_id_size;
  const int kSclPin = 22;
  const int kSdaPin = 21;
  /** Enabled sensors of the glove. */
  static const GloveSensors sensors_;

  void setUpSensors();
  void calibrateSensors();
};

#endif  // GLOVE_H
