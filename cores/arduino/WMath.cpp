/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2024 Ayush Singh <ayush@beagleboard.org>
 * Copyright (c) 2026 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

#ifndef CONFIG_MINIMAL_LIBC_RAND

#include <stdlib.h>

void randomSeed(unsigned long seed) {
	srand(seed);
}

long random(long min, long max) {
	return rand() % (max - min) + min;
}

long random(long max) {
	return rand() % max;
}

#endif
