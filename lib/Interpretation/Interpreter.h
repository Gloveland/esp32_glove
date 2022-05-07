#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "FreeRTOS.h"
#include "freertos/semphr.h"
#include "GloveMeasurements.h"
#include "../Communication/Ble/BleCommunicator.h"

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
    uint32_t run_inference_every_ms = 100;
    
    SemaphoreHandle_t mutex;
    float *buffer;
    float *inference_buffer;
    BleCommunicator *bleCommunicator;

  public:
    explicit Interpreter(BleCommunicator *bleCommunicator);

    void startInterpretations();

    void processGloveMeasurements(GloveMeasurements gloveMeasurements);

    void stopInterpretations();

    ~Interpreter();

  private:
    TaskHandle_t inference_task_handler_ = nullptr;
    static void startInferenceTaskImpl(void *_this);
    [[noreturn]] void taskInference();
};

#endif // INTERPRETER_H_
