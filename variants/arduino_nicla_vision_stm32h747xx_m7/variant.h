/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "pure_analog_pins.h"

#define LEDR    (4u) /* D4 - Red LED PE_3 */
#define LEDG    (5u) /* D5 - Green LED PC_13 */
#define LEDB    (6u) /* D6 - Blue LED PF_4 */

/* SPI interface for LSM6DSOX IMU */
#define LSM6DS_DEFAULT_SPI    SPI1
#define LSM6DS_INT    (1u)
#define PIN_SPI_SS1    0u  /* CS not used */

// TODO: correctly handle these legacy defines
#define MOSI    0
#define MISO    0
#define SCK     0
#define SS      0

#define SDA     0
#define SCL     0

#define SE05X_ENABLE_GPIO (7u)
