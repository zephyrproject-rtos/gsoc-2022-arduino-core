/*
 * Copyright (c) 2024 Ayush Singh <ayush@beagleboard.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void enableInterrupt(pin_size_t);
void disableInterrupt(pin_size_t);
#ifdef CONFIG_PINCTRL_DYNAMIC
void _reinit_peripheral_if_needed(pin_size_t pin, const struct device *dev);
#endif

#ifdef __cplusplus
} // extern "C"
#endif
