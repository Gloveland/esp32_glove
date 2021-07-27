
#include <Utils.h>

void clearInput() {
  while (Serial.available()) {
    Serial.read();
    /* clear the input buffer */
  }
}

void waitAnyUserInput(String msg) {
  clearInput();
  Serial.println(msg);
  while (!Serial.available()) {
    /* wait for a character */
  }
  clearInput();
}
