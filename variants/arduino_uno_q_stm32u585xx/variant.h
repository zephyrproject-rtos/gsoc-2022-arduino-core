/*
 * Copyright (c) 2022 Dhruva Gole
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if !ARDUINO_LIBRARY_DISCOVERY_PHASE
#if __has_include("Arduino_RouterBridge.h")
#if __has_include("routerbridge_provides_serial.h")
#define ARDUINO_ROUTERBRIDGE_PROVIDES_SERIAL
#else
#error "Please update the Arduino_RouterBridge library to the latest version to ensure Serial support on this board."
#endif
#endif
#endif

// TODO: correctly handle these legacy defines
#define MOSI    0
#define MISO    0
#define SCK     0
#define SS      0
#define SDA     0
#define SCL     0

#define AR_DEFAULT          0
#define AR_INTERNAL2V5      1
#define AR_INTERNAL2V05     2
#define AR_INTERNAL1V5      3
#define AR_INTERNAL1V8      4
#define AR_EXTERNAL         5
#define AR_INTERNAL         AR_INTERNAL2V5

// RGB LEDs Pin Map
#define LED3_R DIGITAL_PIN_GPIOS_FIND_NODE(DT_NODELABEL(led3_red))
#define LED3_G DIGITAL_PIN_GPIOS_FIND_NODE(DT_NODELABEL(led3_green))
#define LED3_B DIGITAL_PIN_GPIOS_FIND_NODE(DT_NODELABEL(led3_blue))

#define LED4_R DIGITAL_PIN_GPIOS_FIND_NODE(DT_NODELABEL(led4_red))
#define LED4_G DIGITAL_PIN_GPIOS_FIND_NODE(DT_NODELABEL(led4_green))
#define LED4_B DIGITAL_PIN_GPIOS_FIND_NODE(DT_NODELABEL(led4_blue))

#include "../common/gpio_lowlevel_stm32.h"
