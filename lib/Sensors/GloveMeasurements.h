#ifndef GLOVE_MEASUREMENT_H_
#define GLOVE_MEASUREMENT_H_

#include "Finger.h"
#include "ImuSensorMeasurement.h"

class GloveMeasurements {

  public: 
    ImuSensorMeasurement * thumb_imu_measurement;
    ImuSensorMeasurement * index_imu_measurement;
    ImuSensorMeasurement * middle_imu_measurement;
    ImuSensorMeasurement * ring_imu_measurement;
    ImuSensorMeasurement * pinky_imu_measurement;
    GloveMeasurements();
    ImuSensorMeasurement &getSensor(Finger::Value value);
    void setSensorMeasurement(const Finger::Value &value, ImuSensorMeasurement measurement);
    ~GloveMeasurements();

    JSONVar toJson() const;
};

#endif // GLOVE_MEASUREMENT_H_
