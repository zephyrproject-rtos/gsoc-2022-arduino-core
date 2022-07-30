/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"

/*
 *  We have initialised all pins by default to pull down
 *  inorder to not have random floating voltages in pins
 *  The ACTIVE_HIGH flag is set so that A low physical
 *  level on the pin will be interpreted as value 0.
 *  A high physical level will be interpreted as value 1
 */
void pinMode(pin_size_t pinNumber, PinMode pinMode) {
  if (pinMode == INPUT || pinMode == INPUT_PULLDOWN) { // input mode
    gpio_pin_configure_dt(arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_PULL_DOWN | GPIO_ACTIVE_HIGH);
  } else { // output mode
    gpio_pin_configure_dt(arduino_pins[pinNumber],
                          GPIO_OUTPUT_LOW | GPIO_PULL_DOWN | GPIO_ACTIVE_HIGH);
  }
}

void digitalWrite(pin_size_t pinNumber, PinStatus status) {
  gpio_pin_set_dt(arduino_pins[pinNumber], status);
}

PinStatus digitalRead(pin_size_t pinNumber) {
  return (gpio_pin_get_dt(arduino_pins[pinNumber]) == 1) ? HIGH : LOW;
}

void delay(unsigned long ms) { k_sleep(K_MSEC(ms)); }

void delayMicroseconds(unsigned int us) { k_sleep(K_USEC(us)); }