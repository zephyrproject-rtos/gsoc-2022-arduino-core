/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include "wiring_private.h"

namespace zephyr {
namespace arduino {

// create an array of arduino_pins with functions to reinitialize pins if needed
#if DT_NODE_HAS_PROP(DT_PATH(zephyr_user), digital_pin_gpios)
const struct device *pinmux_array[DT_PROP_LEN(DT_PATH(zephyr_user), digital_pin_gpios)] = {nullptr};
#else
const struct device *pinmux_array[end_accum(ARRAY_SIZE(gpio_ports))] = {nullptr};
#endif

void _reinit_peripheral_if_needed(pin_size_t pin, const struct device *dev) {
	if (pinmux_array[pin] != dev) {
		pinmux_array[pin] = dev;
		if (dev != NULL) {
			dev->ops.init(dev);
		}
	}
}

} // namespace arduino
} // namespace zephyr

using namespace zephyr::arduino;

void yield(void) {
	k_yield();
}

unsigned long micros(void) {
#ifdef CONFIG_TIMER_HAS_64BIT_CYCLE_COUNTER
	return k_cyc_to_us_floor32(k_cycle_get_64());
#else
	return k_cyc_to_us_floor32(k_cycle_get_32());
#endif
}

unsigned long millis(void) {
	return k_uptime_get_32();
}

const struct device *digitalPinToPortDevice(pin_size_t pinNumber) {
	return local_gpio_port(pinNumber);
}

int digitalPinToPinIndex(pin_size_t pinNumber) {
	return local_gpio_pin(pinNumber);
}
