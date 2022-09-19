/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

#define LED_BUILTIN 22

const static struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(sercom0));
