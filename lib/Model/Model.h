#ifndef MODEL_H
#define MODEL_H

using namespace std;

struct Acceleration {
  float x, y, z;
};

struct Gyro {
  float x, y, z;
};

struct Inclination {
  float roll;
  float pitch;
  float yaw;
};

struct Finger {
  Acceleration acc;
  Gyro gyro;
  Inclination inclination;
};

struct Hand {
  Finger thump;
  //Finger index;
  //Finger middle;
  //Finger ring;
  //Finger pinky;
};

struct Movement {
  string device_id;
  int event_num;
  Hand hand;
};


#endif //MODEL_H
