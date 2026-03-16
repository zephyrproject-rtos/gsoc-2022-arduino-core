/*
 * Copyright (c) 2026 KurtE
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

uint8_t shiftIn(pin_size_t dataPin, uint8_t clockPin, BitOrder bitOrder) {
	uint8_t value = 0;
	uint8_t mask;

	if (bitOrder == LSBFIRST) {
		for (mask = 0x01; mask; mask <<= 1) {
			digitalWrite(clockPin, HIGH);
			if (digitalRead(dataPin)) {
				value |= mask;
			}
			digitalWrite(clockPin, LOW);
		}
	} else {
		for (mask = 0x80; mask; mask >>= 1) {
			digitalWrite(clockPin, HIGH);
			if (digitalRead(dataPin)) {
				value |= mask;
			}
			digitalWrite(clockPin, LOW);
		}
	}

	return value;
}

void shiftOut(pin_size_t dataPin, uint8_t clockPin, BitOrder bitOrder, uint8_t val) {
	uint8_t mask;

	if (bitOrder == LSBFIRST) {
		for (mask = 0x01; mask; mask <<= 1) {
			digitalWrite(dataPin, !!(val & mask) ? HIGH : LOW);
			digitalWrite(clockPin, HIGH);
			digitalWrite(clockPin, LOW);
		}
	} else {
		for (mask = 0x80; mask; mask >>= 1) {
			digitalWrite(dataPin, !!(val & mask) ? HIGH : LOW);
			digitalWrite(clockPin, HIGH);
			digitalWrite(clockPin, LOW);
		}
	}
}
