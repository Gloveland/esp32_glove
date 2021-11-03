#ifndef INTERPRETER_H_
#define INTERPRETER_H_

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
    bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
    uint32_t run_inference_every_ms = 200;
     
    float *buffer;
    float *inference_buffer;

  public:
    explicit Interpreter();

    void startInterpretations();

    void processGloveMeasurements(GloveMeasurements gloveMeasurements);

    void stopInterpretations();

    ~Interpreter();

  private:
    TaskHandle_t inference_task_handler_ = nullptr;
    static void startInferenceTaskImpl(void *_this);
    [[noreturn]] void taskInference();
    void ei_printf(const char *format, ...);
};

#endif // INTERPRETER_H_
