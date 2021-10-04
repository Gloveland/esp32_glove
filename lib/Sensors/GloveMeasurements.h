#ifndef GLOVE_MEASUREMENT_H_
#define GLOVE_MEASUREMENT_H_

#include "Finger.h"
#include "ImuSensorMeasurement.h"

class GloveMeasurements {
 private:
  
  const static std::string kglove_mesurements_packet_format;
  std::map<const Finger::Value, ImuSensorMeasurement> imuSensorMeasurementMap_;

 public:
  static const int kImuSensorsAmount;
  GloveMeasurements();
  void toPackage(int events_count, char *glove_measurement_buffer_,
                 const int size);
  void setSensorMeasurementsMap(const std::map<const Finger::Value, ImuSensorMeasurement>
                                imuSensorMeasurementMap);
  ImuSensorMeasurement getSensor(const Finger::Value finger);
  bool isNotComplete();

  ~GloveMeasurements();
};

#endif  // GLOVE_MEASUREMENT_H_
