#ifndef IMU_SENSOR_MEASUREMENT_H_
#define IMU_SENSOR_MEASUREMENT_H_

#include <Arduino_JSON.h>
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Inclination.h"

/**
 * The measurement of an IMU sensor.
 */
struct ImuSensorMeasurement {
  Acceleration acc;
  Gyro gyro;
  Inclination inclination;
  float temperature{};

  JSONVar toJson() const {
    JSONVar json_acc;
    JSONVar json_gyro;
    JSONVar json_inclination;
    JSONVar json_measurement;

    json_acc["x"] = acc.x;
    json_acc["y"] = acc.y;
    json_acc["z"] = acc.z;

    json_gyro["x"] = gyro.x;
    json_gyro["y"] = gyro.y;
    json_gyro["z"] = gyro.z;

    json_inclination["roll"] = inclination.roll;
    json_inclination["pitch"] = inclination.pitch;
    json_inclination["yaw"] = inclination.yaw;

    json_measurement["acc"] = json_acc;
    json_measurement["gyro"] = json_gyro;
    json_measurement["inclination"] = json_inclination;

    //TODO(Darius): enable temperature below when it's ready in the app.
    //  json_measurement["temperature"] = temperature;

    return json_measurement;
  }
};

#endif // IMU_SENSOR_MEASUREMENT_H_