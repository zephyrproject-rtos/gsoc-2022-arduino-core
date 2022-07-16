/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* All the pins that are 100 + x are gpio1 pins and < 100 are in gpio0 */
#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/zephyr.h>

#define LED_BUILTIN 13

static struct gpio_dt_spec d0 = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d0_gpios);
static struct gpio_dt_spec d1 = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d1_gpios);
static struct gpio_dt_spec d2 = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d2_gpios);

static struct gpio_dt_spec *arduino_pins[3] = { &d0, &d1, &d2 };

enum digitalPins {
  D0 = 3,
  D1 = 10,
  D2 = 11,
  D3 = 112,
  D4 = 115,
  D5 = 113,
  D6 = 114,
  D7 = 9,
  D8 = 10,
  D9 = 27,
  D10 = 102,
  D11 = 101,
  D12 = 108,
  D13 = 13,
  D14 = 4,
  D15 = 5,
  D16 = 30,
  D17 = 29,
  D18 = 14,
  D19 = 15,
  D20 = 28,
  D21 = 103
};