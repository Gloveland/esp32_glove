#include "Interpreter.h"
#include <RightGloveLSA_inferencing.h>

// https://os.mbed.com/docs/mbed-os/v6.15/apis/thread.html

Interpreter::Interpreter() {
    this->buffer = new float[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
    this->inference_buffer = new float[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
}

Interpreter::~Interpreter() {
    delete this->buffer;
    delete this->inference_buffer;
}

void Interpreter::startInterpretations() {
  this->stopInterpretations();
  log_i("Starting inference task.");
  xTaskCreatePinnedToCore(this->startInferenceTaskImpl, "Inference task", 10000,
                          this, 1, &inference_task_handler_, 0);
}

void Interpreter::processGloveMeasurements(
    GloveMeasurements gloveMeasurements) {
  numpy::roll(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, -6);
  Acceleration indexAcceleration =
      gloveMeasurements.getSensor(Finger::Value::kIndex).getAcc();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 6] = indexAcceleration.getX();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 5] = indexAcceleration.getY();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 4] = indexAcceleration.getZ();

  Acceleration middleAcceleration =
      gloveMeasurements.getSensor(Finger::Value::kMiddle).getAcc();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 3] = middleAcceleration.getX();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 2] = middleAcceleration.getY();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1] = middleAcceleration.getZ();
}

void Interpreter::stopInterpretations() {
  if (this->inference_task_handler_ != nullptr) {
    log_i("Stopping inference task.");
    vTaskDelete(inference_task_handler_);
    this->inference_task_handler_ = nullptr;
    log_i("Inference task stopped.");
  }
}

/**
 * @brief      Printf function uses vsnprintf and output using Arduino Serial
 *
 * @param[in]  format     Variable argument list
 */
void Interpreter::ei_printf(const char *format, ...) {
  static char print_buf[1024] = {0};

  va_list args;
  va_start(args, format);
  int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
  va_end(args);

  if (r > 0) {
    Serial.write(print_buf);
  }
}

void Interpreter::startInferenceTaskImpl(void *_this) {
  log_i("Task interpretation inference...");
  if (_this == nullptr) {
    log_i("Error creating task, null task parameter");
  }
  ((Interpreter *)_this)->taskInference();
}

[[noreturn]] void Interpreter::taskInference() {
  log_i("Task 'inference' running on code %d", xPortGetCoreID());
  delay((EI_CLASSIFIER_INTERVAL_MS * EI_CLASSIFIER_RAW_SAMPLE_COUNT) + 100);

  // This is a structure that smoothens the output result
  // With the default settings 70% of readings should be the same before
  // classifying.
  ei_classifier_smooth_t smooth;
  ei_classifier_smooth_init(&smooth, 10 /* no. of readings */,
                            7 /* min. readings the same */,
                            0.8 /* min. confidence */, 0.3 /* max anomaly */);

  while (1) {
    // copy the buffer
    memcpy(inference_buffer, buffer,
           EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * sizeof(float));

    // Turn the raw buffer in a signal which we can the classify
    signal_t signal;
    int err = numpy::signal_from_buffer(
        inference_buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
      ei_printf("Failed to create signal from buffer (%d)\n", err);
    }

    // Run the classifier
    ei_impulse_result_t result = {0};

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
      ei_printf("ERR: Failed to run classifier (%d)\n", err);
    }

    // print the predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
              result.timing.dsp, result.timing.classification,
              result.timing.anomaly);
    ei_printf(": ");

    // ei_classifier_smooth_update yields the predicted label
    const char *prediction = ei_classifier_smooth_update(&smooth, &result);
    ei_printf("%s ", prediction);
    // print the cumulative results
    ei_printf(" [ ");
    for (size_t ix = 0; ix < smooth.count_size; ix++) {
      ei_printf("%u", smooth.count[ix]);
      if (ix != smooth.count_size + 1) {
        ei_printf(", ");
      } else {
        ei_printf(" ");
      }
    }
    ei_printf("]\n");

    delay(run_inference_every_ms);
  }

  ei_classifier_smooth_free(&smooth);
}