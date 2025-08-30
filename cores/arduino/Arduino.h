/*
 * Copyright (c) 2022 Dhruva Gole
 * Copyright (c) 2025 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <api/ArduinoAPI.h>

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/i2c.h>

#define ZARD_ADD_NGPIOS(i, n, p) DT_PROP(DT_PROP_BY_IDX(n, p, i), ngpios) +
#define ZARD_ACCUM_NGPIOS(n, p, i, nd)                                                             \
  COND_CODE_1(DT_SAME_NODE(DT_PROP_BY_IDX(n, p, i), nd),                                           \
			   (LISTIFY(i, ZARD_ADD_NGPIOS, (), n, p)), ())
#define ZARD_GLOBAL_GPIO_NUM(n, p, i)                                                              \
  DT_FOREACH_PROP_ELEM_VARGS(DT_PATH(zephyr_user), gpio_ports, ZARD_ACCUM_NGPIOS,                  \
                             DT_PHANDLE_BY_IDX(n, p, i))                                           \
  DT_PHA_BY_IDX(n, p, i, pin)

/*
 * expand as
 * enum digitalPins { D0, D1, ... };
 */
#define ZARD_DN_ENUMS(n, p, i) D##i = ZARD_GLOBAL_GPIO_NUM(n, p, i)
enum digitalPins {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), digital_pin_gpios, ZARD_DN_ENUMS, (, )),
  NUM_OF_DIGITAL_PINS
};

#ifdef CONFIG_ADC
#define ZARD_AN_ENUMS(n, p, i) A##i = ZARD_GLOBAL_GPIO_NUM(n, p, i)
enum analogPins {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), adc_pin_gpios, ZARD_AN_ENUMS, (, )),
  NUM_OF_ANALOG_PINS
};
#endif

void interrupts(void);
void noInterrupts(void);

int digitalPinToInterrupt(pin_size_t pin);

#include <variant.h>

#if !defined(LED_BUILTIN) && DT_NODE_EXISTS(DT_ALIAS(led0))
#define LED_BUILTIN ZARD_GLOBAL_GPIO_NUM(DT_ALIAS(led0), gpios, 0)
#endif // LED_BUILTIN

#ifdef __cplusplus
#include <zephyrPrint.h>
#include <zephyrSerial.h>
#endif // __cplusplus
