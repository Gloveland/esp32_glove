#include "Interpreter.h"

#include <RightGloveLSA_inferencing.h>

#include <sstream>

Interpreter::Interpreter(BleCommunicator *bleCommunicator) {
  this->bleCommunicator = bleCommunicator;
  this->mutex = xSemaphoreCreateMutex();
  this->buffer = new float[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE]();
  this->inference_buffer = new float[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE]();
}

Interpreter::~Interpreter() {
  delete this->buffer;
  delete this->inference_buffer;
}

void Interpreter::startInterpretations() {
  this->stopInterpretations();
  log_i("Starting inference task.");
  xTaskCreatePinnedToCore(this->startInferenceTaskImpl, "Inference task", 10000,
                          this, 1, &inference_task_handler_, 1);
}

void Interpreter::processGloveMeasurements(
    GloveMeasurements gloveMeasurements) {
  Acceleration thumbAcceleration =
      gloveMeasurements.getSensor(Finger::Value::kThumb).getAcc();
  Acceleration indexAcceleration =
      gloveMeasurements.getSensor(Finger::Value::kIndex).getAcc();
  Acceleration middleAcceleration =
      gloveMeasurements.getSensor(Finger::Value::kRing).getAcc();
  Acceleration ringAcceleration =
      gloveMeasurements.getSensor(Finger::Value::kMiddle).getAcc();

  Acceleration pinkyAcceleration =
      gloveMeasurements.getSensor(Finger::Value::kPinky).getAcc();
  xSemaphoreTake(mutex, portMAX_DELAY);
  int samplesPerFrame = EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME;
  numpy::roll(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, -samplesPerFrame);

  assert(samplesPerFrame == 15);
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      thumbAcceleration.getX();
  assert(samplesPerFrame == 14);
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      thumbAcceleration.getY();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      thumbAcceleration.getZ();

  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      indexAcceleration.getX();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      indexAcceleration.getY();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      indexAcceleration.getZ();

  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      middleAcceleration.getX();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      middleAcceleration.getY();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      middleAcceleration.getZ();

  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      ringAcceleration.getX();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      ringAcceleration.getY();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      ringAcceleration.getZ();

  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      pinkyAcceleration.getX();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      pinkyAcceleration.getY();
  buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - samplesPerFrame--] =
      pinkyAcceleration.getZ();
  xSemaphoreGive(mutex);
  std::stringstream log;
  log << "Accel: [" << indexAcceleration.getX() << ", "
      << indexAcceleration.getY() << ", " << indexAcceleration.getZ() << "]";
  bleCommunicator->sendMeasurements(log.str());
}

void Interpreter::stopInterpretations() {
  if (this->inference_task_handler_ != nullptr) {
    log_i("Stopping inference task.");
    vTaskDelete(inference_task_handler_);
    this->inference_task_handler_ = nullptr;
    log_i("Inference task stopped.");
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
    xSemaphoreTake(mutex, portMAX_DELAY);
    // copy the buffer
    memcpy(inference_buffer, buffer,
           EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * sizeof(float));
    xSemaphoreGive(mutex);

    // Turn the raw buffer in a signal which we can the classify
    signal_t signal;
    int err = numpy::signal_from_buffer(
        inference_buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
      log_e("Failed to create signal from buffer (%d)\n", err);
      bleCommunicator->sendInterpretation(
          "Failed to create signal from buffer");
      delay(5000);
    }

    // Run the classifier
    ei_impulse_result_t result = {0};

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
      log_e("ERR: Failed to run classifier (%d)\n", err);
      bleCommunicator->sendInterpretation("ERR: Failed to run classifier.");
      delay(5000);
    }

    // ei_classifier_smooth_update yields the predicted label
    const char *prediction = ei_classifier_smooth_update(&smooth, &result);
    std::stringstream predictionsStream;
    predictionsStream << "(DSP: " << result.timing.dsp
                      << " ms., Classification: "
                      << result.timing.classification
                      << " ms., Anomaly: " << result.timing.anomaly << " ms.)";
    // print the cumulative results

    std::stringstream resultsStream;
    resultsStream << "Prediction: " << prediction << " [";
    for (size_t ix = 0; ix < smooth.count_size; ix++) {
      resultsStream << " " << (int)smooth.count[ix];
      if (ix != smooth.count_size + 1) {
        resultsStream << ",";
      }
    }
    resultsStream << "]\n";

    std::string message;
    message = resultsStream.str() + "\n" + predictionsStream.str();
    log_i("%s", message);
    bleCommunicator->sendInterpretation(message);
    delay(run_inference_every_ms);
  }

  ei_classifier_smooth_free(&smooth);
}
