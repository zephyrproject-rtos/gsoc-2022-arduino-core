/*
 * Copyright (c) 2025 Purva Yeshi <purvayeshi550@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include "EEPROM.h" 

void setup() {

  int data = 1234;  // Example data to store
  int read_data = 0;

  /* Initialize serial communication */
  Serial.begin(115200);
  while (!Serial) {
    ; /* Wait for serial port to connect (needed for boards with native USB) */
  }
  Serial.println("Serial communication initialized");

  /* Initialize EEPROM/NVS */ 
  if (EEPROM.nvs_init() < 0) { 
    Serial.println("NVS initialization failed");
    return;
  }
  Serial.println("NVS initialized");

  /* Write data to EEPROM */
  if (EEPROM.write_data(1, &data, sizeof(data)) < 0) { 
    Serial.println("Failed to write data");
  } else {
    Serial.println("Data written successfully");
  }

  /* Read data from EEPROM */
  if (EEPROM.read_data(1, &read_data, sizeof(read_data)) > 0) { 
    Serial.print("Data read: ");
    Serial.println(read_data);
  } else {
    Serial.println("Failed to read data");
  }
}

void loop() {

}
