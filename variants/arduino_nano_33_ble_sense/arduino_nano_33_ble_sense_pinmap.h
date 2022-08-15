/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* All the pins that are 100 + x are gpio1 pins and < 100 are in gpio0 */
#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/zephyr.h>
<<<<<<< HEAD
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
=======
>>>>>>> 3630ff9 (fix variant folders spellings)

#define LED_BUILTIN 13

static struct gpio_dt_spec d0 = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d0_gpios);
static struct gpio_dt_spec d1 = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d1_gpios);
static struct gpio_dt_spec d2 = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d2_gpios);
static struct gpio_dt_spec d3 = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d3_gpios);
static struct gpio_dt_spec d4 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d4_gpios);
static struct gpio_dt_spec d5 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d5_gpios);
static struct gpio_dt_spec d6 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d6_gpios);
static struct gpio_dt_spec d7 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d7_gpios);
static struct gpio_dt_spec d8 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d8_gpios);
static struct gpio_dt_spec d9 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d9_gpios);
static struct gpio_dt_spec d10 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d10_gpios);
static struct gpio_dt_spec d11 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d11_gpios);
static struct gpio_dt_spec d12 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d12_gpios);
static struct gpio_dt_spec d13 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d13_gpios);
static struct gpio_dt_spec d14 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d13_gpios);
static struct gpio_dt_spec d15 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d15_gpios);
static struct gpio_dt_spec d16 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d16_gpios);
static struct gpio_dt_spec d17 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d17_gpios);
static struct gpio_dt_spec d18 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d18_gpios);
static struct gpio_dt_spec d19 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d19_gpios);
static struct gpio_dt_spec d20 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d20_gpios);
static struct gpio_dt_spec d21 =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), d21_gpios);

static struct gpio_dt_spec *arduino_pins[22] = {
    &d0,  &d1,  &d2,  &d3,  &d4,  &d5,  &d6,  &d7,  &d8,  &d9,  &d10,
    &d11, &d12, &d13, &d14, &d15, &d16, &d17, &d18, &d19, &d20, &d21};

enum digitalPins {
  D0,
  D1,
  D2,
  D3,
  D4,
  D5,
  D6,
  D7,
  D8,
  D9,
  D10,
  D11,
  D12,
  D13,
  D14,
  D15,
  D16,
  D17,
  D18,
  D19,
  D20,
  D21
};
<<<<<<< HEAD

const static struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
=======
>>>>>>> 3630ff9 (fix variant folders spellings)
