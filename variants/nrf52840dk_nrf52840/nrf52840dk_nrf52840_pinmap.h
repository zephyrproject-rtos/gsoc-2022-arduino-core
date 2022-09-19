/*
 * Copyright (c) 2022 Mike Szczys
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __NRF52840DK_NRF52840_PINMAP_H__
#define __NRF52840DK_NRF52840_PINMAP_H__

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

#define LED_BUILTIN 22

const static struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(arduino_i2c));

#endif
