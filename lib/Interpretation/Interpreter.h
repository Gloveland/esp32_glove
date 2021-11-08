#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "Glove.h"
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
 private:
 static const bool kDebugNN_; // Set this to true to see e.g. features generated
                          // from the raw signal
  Glove * glove_;
  uint32_t run_inference_every_ms = 200;

 public:
  Interpreter(Glove * glove);
  std::string classify();
  ~Interpreter();
};

#endif  // INTERPRETER_H_
