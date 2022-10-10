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

#define DIGITAL_PIN_EXISTS(n, p, i, dev, num)                                                      \
	(((dev == DT_REG_ADDR(DT_PHANDLE_BY_IDX(n, p, i))) &&                                      \
	  (num == DT_PHA_BY_IDX(n, p, i, pin)))                                                    \
		 ? 1                                                                               \
		 : 0)

#define DIGITAL_PIN_CHECK_UNIQUE(i, _)                                                             \
	((DT_FOREACH_PROP_ELEM_SEP_VARGS(                                                          \
		 DT_PATH(zephyr_user), digital_pin_gpios, DIGITAL_PIN_EXISTS, (+),                 \
		 DT_REG_ADDR(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), digital_pin_gpios, i)),       \
		 DT_PHA_BY_IDX(DT_PATH(zephyr_user), digital_pin_gpios, i, pin))) == 1)

#if !LISTIFY(DT_PROP_LEN(DT_PATH(zephyr_user), digital_pin_gpios), DIGITAL_PIN_CHECK_UNIQUE, (&&))
#error "digital_pin_gpios has duplicate definition"
#endif

#undef DIGITAL_PIN_CHECK_UNIQUE

#define DN_ENUMS(n, p, i) D##i = i

/*
 * expand as
 * enum digitalPins { D0, D1, ... LED... NUM_OF_DIGITAL_PINS };
 */
enum digitalPins {
	DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), digital_pin_gpios, DN_ENUMS, (, )),
	NUM_OF_DIGITAL_PINS
};

const struct gpio_dt_spec arduino_pins[] = {DT_FOREACH_PROP_ELEM_SEP(
	DT_PATH(zephyr_user), digital_pin_gpios, GPIO_DT_SPEC_GET_BY_IDX, (, ))};

#ifdef CONFIG_ADC

#define AN_ENUMS(n, p, i) A ## i = DT_PROP_BY_IDX(n, p, i),
enum analogPins { DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user),
				       io_channel_pins, AN_ENUMS) };

#endif
