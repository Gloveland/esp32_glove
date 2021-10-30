#ifndef ABSTRACT_TASK_MANAGER_H
#define ABSTRACT_TASK_MANAGER_H

class AbstractTasksManager {
 public:
  /** Init Bluetooth low energy services. */
  virtual void initBleService();
  /** Starts the data collection task. */
  virtual void startDataCollectionTask();
  /**
   * Stop the ongoing task, either the interpretation task or the data
   * collection task. Nothing happens if there is not an ongoing task.
   */
  virtual void stopRunningTask();
  virtual ~AbstractTasksManager(){};
};
#endif  // ABSTRACT_TASK_MANAGER_H