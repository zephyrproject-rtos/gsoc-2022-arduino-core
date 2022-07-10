/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

void setup() { printk("Hello World! %s\n", CONFIG_BOARD); }
void loop() {
  printk("\nInside Loop...\n");
  delay(1000); // 1 second delay
}
