#ifndef GLOVE_MEASUREMENT_H_
#define GLOVE_MEASUREMENT_H_

#include "Finger.h"
#include "ImuSensorMeasurement.h"

class GloveMeasurements {

  private:
    const static std::string kglove_mesurements_packet_format;
    ImuSensorMeasurement * thumb_imu_measurement_;
    ImuSensorMeasurement * index_imu_measurement_;
    ImuSensorMeasurement * middle_imu_measurement_;
    ImuSensorMeasurement * ring_imu_measurement_;
    ImuSensorMeasurement * pinky_imu_measurement_;

  public: 
    
    GloveMeasurements();
    void toPackage(int events_count,
                                  char *glove_measurement_buffer_, const int size) ;
    ImuSensorMeasurement &getSensor(Finger::Value value);
    bool isNotComplete();
    void setSensorMeasurement(const Finger::Value &value, ImuSensorMeasurement measurement);
    ~GloveMeasurements();

};

#endif // GLOVE_MEASUREMENT_H_
