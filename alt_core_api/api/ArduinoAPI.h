/*
 * Copyright (c) 2026 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>

#include "common_types.h"

typedef gpio_port_pins_t pin_size_t;
typedef uint8_t byte;

typedef void (*voidFuncPtr)(void);
typedef void (*voidFuncPtrParam)(void *);

void setup(void);
void loop(void);
void delay(unsigned long);

namespace arduino {}

using namespace arduino;

template <class T> constexpr T min(const T &a, const T &b) {
	return (a < b) ? a : b;
}
