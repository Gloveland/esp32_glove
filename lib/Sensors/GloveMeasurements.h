#ifndef GLOVE_MEASUREMENT_H_
#define GLOVE_MEASUREMENT_H_

#include "Finger.h"
#include "ImuSensorMeasurement.h"

class GloveMeasurements {
 private:
  const static int kMtu;
  const static std::string kMeasurementPacketFormat;

  const static std::string kGloveMeasurementsPacketFormat;
  std::map<const Finger::Value, ImuSensorMeasurement> imuSensorMeasurementMap_;

 public:
  static const int kImuSensorsAmount;
  GloveMeasurements();
  std::string toPackage(int eventsCount, int elapsedTime);
  void setSensorMeasurementsMap(
      const std::map<const Finger::Value, ImuSensorMeasurement>
          imuSensorMeasurementMap);
  ImuSensorMeasurement getSensor(const Finger::Value finger);
  bool isNotComplete();

  ~GloveMeasurements();
};

#endif  // GLOVE_MEASUREMENT_H_
