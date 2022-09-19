/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#ifdef CONFIG_BOARD_ARDUINO_NANO_33_BLE
#include <arduino_nano_33_ble_pinmap.h>
#endif // CONFIG_BOARD_ARDUINO_NANO_33_BLE
#ifdef CONFIG_BOARD_ARDUINO_NANO_33_BLE_SENSE
#include <arduino_nano_33_ble_sense_pinmap.h>
#endif // CONFIG_BOARD_ARDUINO_NANO_33_BLE_SENSE
#ifdef CONFIG_BOARD_ARDUINO_NANO_33_IOT
#include <arduino_nano_33_iot_pinmap.h>
#endif /* CONFIG_BOARD_ARDUINO_NANO_33_IOT */
#ifdef CONFIG_BOARD_NRF52840DK_NRF52840
#include "nrf52840dk_nrf52840_pinmap.h"
#endif /* CONFIG_BOARD_NRF52840DK_NRF52840 */
#ifdef CONFIG_BOARD_ARDUINO_MKRZERO
#include "arduino_mkrzero_pinmap.h"
#endif // CONFIG_BOARD_ARDUINO_MKRZERO

#define MAX_DIGITAL_PINS 255

#define DN_ENUMS(n, _) COND_CODE_1(DT_NODE_HAS_PROP(DT_PATH(zephyr_user), \
			d ## n ## _gpios), (D ## n ,), ())

#define LABEL_UPPER_TOKEN_COMMA(nodeid) DT_STRING_UPPER_TOKEN(nodeid, label),

/*
 * expand as
 * enum digitalPins { D0, D1, ... LED... NUM_OF_DIGITAL_PINS };
 */
enum digitalPins { LISTIFY(MAX_DIGITAL_PINS, DN_ENUMS, ())
		   DT_FOREACH_CHILD(DT_PATH(leds), LABEL_UPPER_TOKEN_COMMA)
		   NUM_OF_DIGITAL_PINS };


#define NUMBERED_GPIO_DT_SPEC(n, _) \
	COND_CODE_1(DT_NODE_HAS_PROP(DT_PATH(zephyr_user), d ## n ## _gpios), \
	      (GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d ## n ## _gpios),), ())

#define LABELED_GPIO_DT_SPEC(nodeid) GPIO_DT_SPEC_GET_BY_IDX(nodeid, gpios, 0),

static struct gpio_dt_spec arduino_pins[NUM_OF_DIGITAL_PINS] = {
	LISTIFY(MAX_DIGITAL_PINS, NUMBERED_GPIO_DT_SPEC, ())
	DT_FOREACH_CHILD(DT_PATH(leds), LABELED_GPIO_DT_SPEC)
};
