/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"
#include "zephyr/kernel.h"
#include <cstdint>
#ifdef CONFIG_LLEXT
#include <zephyr/llext/symbol.h>
#endif

// This function will be overwriten by most variants.
void __attribute__((weak)) initVariant(void) {
}

// This function can be overwriten by one library.
void __attribute__((weak)) __loopHook(void) {
}

int main(void) {
	initVariant();

	setup();

	for (;;) {
		loop();
#if ZARD_GENERIC_SERIAL_COUNT > 0
		if (arduino::serialEventRun) {
			arduino::serialEventRun();
		}
#endif
		__loopHook();
	}

	return 0;
}

#ifdef CONFIG_LLEXT
LL_EXTENSION_SYMBOL(main);
#endif
