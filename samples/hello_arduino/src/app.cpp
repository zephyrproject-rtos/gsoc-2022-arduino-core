/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include "zephyrSerial.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // dummy as of now, need to study and refer https://docs.zephyrproject.org/latest/hardware/peripherals/uart.html
}
void loop() {
  char c = 'D';
  Serial.print(c);
  Serial.println("Hello, World!");
  delay(1000); // 1 second delay
}
