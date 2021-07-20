#ifndef ESP32_GLOVE_CHARACTERISTICCALLBACKS_H
#define ESP32_GLOVE_CHARACTERISTICCALLBACKS_H

#include <BLECharacteristic.h>

/**
 * Callbacks to handle characteristic events such as onWrite or onNotify.
 */
class CharacteristicCallbacks : public BLECharacteristicCallbacks {
 public:
  void onWrite(BLECharacteristic *pCharacteristic) override;
  void onNotify(BLECharacteristic *pCharacteristic) override;
};

#endif  // ESP32_GLOVE_CHARACTERISTICCALLBACKS_H
