#include <ImuSensorMeasurement.h>

ImuSensorMeasurement::ImuSensorMeasurement(Acceleration acceleration, Gyro gyro, Inclination inclination):
    acc(acceleration),
    gyro(gyro),
    inclination(inclination){}

Acceleration ImuSensorMeasurement::getAcc() const{
    return this->acc;
}

Gyro ImuSensorMeasurement::getGyro() const{
    return this->gyro;
}

Inclination ImuSensorMeasurement::getInclination() const{
    return this->inclination;
}

/*

JSONVar ImuSensorMeasurement::toJson() const {
    JSONVar json_acc;
    JSONVar json_gyro;
    JSONVar json_inclination;
    JSONVar json_measurement;

    json_acc["x"] = acc.getX();
    json_acc["y"] = acc.getY();
    json_acc["z"] = acc.getZ();

    json_gyro["x"] = gyro.getX();
    json_gyro["y"] = gyro.getY();
    json_gyro["z"] = gyro.getZ();

    json_inclination["roll"] = inclination.getRoll();
    json_inclination["pitch"] = inclination.getPitch();
    json_inclination["yaw"] = inclination.getYaw();

    json_measurement["acc"] = json_acc;
    json_measurement["gyro"] = json_gyro;
    json_measurement["inclination"] = json_inclination;

    //TODO(Darius): enable temperature below when it's ready in the app.
    //  json_measurement["temperature"] = temperature;

    return json_measurement;
  }
  */

ImuSensorMeasurement::~ImuSensorMeasurement() = default;