
#include <Glove.h>
#include <Utils.h>
#include <esp_task_wdt.h>

#include <sstream>

#include "../lib/TasksManager/TasksManager.h"

TasksManager* tasksManager;

void setup() {
  Serial.begin(9600);
  log_e("Test log level error -DCORE_DEBUG_LEVEL=1");
  log_w("Test log level warnign -DCORE_DEBUG_LEVEL=2");
  log_i("Test log level information -DCORE_DEBUG_LEVEL=3");
  log_d("Test log level debug -DCORE_DEBUG_LEVEL=4");
  log_v("Test log level verbose -DCORE_DEBUG_LEVEL=5");
  pinMode(LED_BUILTIN, OUTPUT);
  esp_task_wdt_init(100, false);

  Glove* glove = new Glove();
  glove->init();
  tasksManager = new TasksManager(glove);
  tasksManager->initBleService();
}

void loop() {
  // loop() runs on core 1
}
