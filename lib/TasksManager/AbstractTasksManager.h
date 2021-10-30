#ifndef ABSTRACT_TASK_MANAGER_H
#define ABSTRACT_TASK_MANAGER_H

class AbstractTasksManager {
 public:
  /** Init Bluetooth low energy services. */
  virtual void initBleService();
  /** Starts the data collection task. */
  virtual void startDataCollectionTask();

  virtual void startBleMeasurementSenderTask();

  /** Starts the interpretation task. */
  virtual void startInterpretationTask();

  /** Starts the calibration task. */
  virtual void startCalibrationTask();
  /**
   * Stop the ongoing task, either the interpretation task or the data
   * collection task. Nothing happens if there is not an ongoing task.
   */
  virtual void stopRunningTask();
  virtual ~AbstractTasksManager(){};
};
#endif  // ABSTRACT_TASK_MANAGER_H