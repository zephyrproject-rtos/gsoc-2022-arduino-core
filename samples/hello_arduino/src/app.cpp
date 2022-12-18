/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
}
void loop() {
	// loop function for continuously executing code
	Serial.println("Hello World!");
	delay(1000);
}
