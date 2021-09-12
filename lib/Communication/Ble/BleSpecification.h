#ifndef BLE_SPECIFICATION_H
#define BLE_SPECIFICATION_H

/** Service for reading measurements or retrieving interpretations from the gloves. */
#define LSA_GLOVES_SERVICE_UUID "7056f14b-02df-4dd8-86fd-0261c7b15c86"

/** Characteristic for commanding the device. */
#define CONTROLLER_CHARACTERISTIC_UUID  "30b7db16-4567-42c5-acc4-2b0270c1e14d"

/** Characteristic for reading measurements from the gloves. */
#define DATA_COLLECTION_CHARACTERISTIC_UUID  "47e62e53-e278-494d-a3f8-ac00973ae0af"

/** Characteristic for receiving interpretations from the gloves. */
#define INTERPRETATION_CHARACTERISTIC_UUID "079b8e74-101b-11ec-82a8-0242ac130003"

#endif // BLE_SPECIFICATION_H