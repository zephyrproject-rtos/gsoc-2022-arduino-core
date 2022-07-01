/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "api/ArduinoAPI.h"

/* Zephyr libraries */
#ifdef CONFIG_BOARD_ARDUINO_NANO_33_BLE_SENSE 
#include "arduino_nano_ble_sense_pinmap.h"
#endif
#ifdef CONFIG_BOARD_PARTICLE_XENON 
#include "particle_xenon_pinmap.h"
#endif
#include <zephyr/zephyr.h>
#include <zephyr/drivers/gpio.h>
