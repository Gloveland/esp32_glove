#ifndef GLOVE_MEASUREMENT_H_
#define GLOVE_MEASUREMENT_H_

#include "Finger.h"
#include "ImuSensorMeasurement.h"

class GloveMeasurements {

  private:
    const static std::string kglove_mesurements_packet_format;
    ImuSensorMeasurement * thumb_imu_measurement;
    ImuSensorMeasurement * index_imu_measurement;
    ImuSensorMeasurement * middle_imu_measurement;
    ImuSensorMeasurement * ring_imu_measurement;
    ImuSensorMeasurement * pinky_imu_measurement;

  public: 
    
    GloveMeasurements();
    void toPackage(int events_count,
                                  char *glove_measurement_buffer, const int size) ;
    ImuSensorMeasurement &getSensor(Finger::Value value);
    void setSensorMeasurement(const Finger::Value &value, ImuSensorMeasurement measurement);
    ~GloveMeasurements();
};

#endif // GLOVE_MEASUREMENT_H_
