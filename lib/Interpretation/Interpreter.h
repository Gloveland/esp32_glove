#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <RightGloveLSA_inferencing.h>
#include <Arduino_LSM9DS1.h>
#include "GloveMeasurements.h"

/** 
 * Class to handle the interpretation logic of the gestures. 
 * 
 * This works with the Edge Impulse provided library. To install it run the
 * following command:
 * 
 * `pio lib install ./edge_impulse_library_name.zip`
 * 
 * where `edge_impulse_library_name` is the name of the zip file provided by
 * edge impulse, located in the root folder of this project.
*/
class Interpreter {
  public:
    void startInterpretations();

    void processGloveMeasurement(GloveMeasurements gloveMeasurements);

    void stopInterpretations();
}

#endif // INTERPRETER_H_