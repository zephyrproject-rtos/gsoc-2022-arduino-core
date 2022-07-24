/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "api/ArduinoAPI.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/zephyr.h>

#ifdef CONFIG_BOARD_ARDUINO_NANO_33_BLE_SENSE
#include "arduino_nano_ble_sense_pinmap.h"
#endif // CONFIG_BOARD_ARDUINO_NANO_33_BLE_SENSE