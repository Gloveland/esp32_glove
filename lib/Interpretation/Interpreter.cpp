#include "Interpreter.h"

#include <RightGloveLSA_inferencing.h>

#include <sstream>
const bool Interpreter::kDebugNN_ = false;


Interpreter::Interpreter(Glove * glove) : glove_(glove) {}

std::string Interpreter::classify() {
  log_i("Starting inferencing...");
  std::stringstream interpretation_stream;

  // Allocate a buffer here for the values we'll read from the IMU
  float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = {0};

  for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
    // Determine the next tick (and then sleep later)
    uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

     GloveMeasurements measurements = this->glove_->readSensors();
     ImuSensorMeasurement index = (measurements.getSensor(Finger::kIndex));
     buffer[ix] = index.getAcc().getX();
     buffer[ix + 1] = index.getAcc().getY();
     buffer[ix + 2] = index.getAcc().getZ();
     delayMicroseconds(next_tick - micros());
   }

   
  // Turn the raw buffer in a signal which we can the classify
  signal_t signal;
  int err = numpy::signal_from_buffer(
      buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
  if (err != 0) {
    log_e("Failed to create signal from buffer (%d)\n", err);
    interpretation_stream << "Failed to create signal from buffer " << err;
    return interpretation_stream.str();
  }
  // Run the classifier
  ei_impulse_result_t result = {0};
  err = run_classifier(&signal, &result,Interpreter::kDebugNN_);
  if (err != EI_IMPULSE_OK) {
    log_e("ERR: Failed to run classifier (%d)", err);
    interpretation_stream << "ERR: Failed to run classifier: " << err;
    return interpretation_stream.str();
  }
  // print the predictions
  log_i("Predictions ");
  log_i("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification,
  result.timing.anomaly);
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    log_i("    %s: %.5f ", result.classification[ix].label,
          result.classification[ix].value);
    interpretation_stream << "Interpretation " <<
  result.classification[ix].label << ":" << result.classification[ix].value;
  }
 
  return interpretation_stream.str();
}

Interpreter::~Interpreter() {}