
#include <Glove.h>
#include <Utils.h>
#include <esp_task_wdt.h>

#include "../lib/TasksManager/TasksManager.h"


void setup() {
  Serial.begin(9600);
  log_e("Test log level error -DCORE_DEBUG_LEVEL=1");
  log_w("Test log level warnign -DCORE_DEBUG_LEVEL=2");
  log_i("Test log level information -DCORE_DEBUG_LEVEL=3");
  log_d("Test log level debug -DCORE_DEBUG_LEVEL=4");
  log_v("Test log level verbose -DCORE_DEBUG_LEVEL=5");
  pinMode(LED_BUILTIN, OUTPUT);
  esp_task_wdt_init(100, false);

  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  delay(1000);
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  delay(1000);
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  delay(1000);
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);
  delay(1000);
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);
  delay(1000);

  digitalWrite(17, LOW);

  Glove* glove = new Glove();
  glove->init();
  TasksManager* tasksManager = new TasksManager(glove);
  tasksManager->initBleService();
}

void loop() {
}  // loop() runs on core 1
