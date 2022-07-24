/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"

void pinMode(pin_size_t pinNumber, PinMode pinMode) {
  if (pinMode == INPUT || pinMode == INPUT_PULLDOWN) { // input mode
    gpio_pin_configure_dt(arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_ACTIVE_LOW);
  } else { // output mode
    gpio_pin_configure_dt(arduino_pins[pinNumber], GPIO_OUTPUT);
  }
}

void digitalWrite(pin_size_t pinNumber, PinStatus status) {
  if (status == HIGH) {
    gpio_pin_set_dt(arduino_pins[pinNumber], GPIO_ACTIVE_HIGH);
  } else if (status == LOW) {
    gpio_pin_set_dt(arduino_pins[pinNumber], GPIO_ACTIVE_LOW);
  }
}

PinStatus digitalRead(pin_size_t pinNumber) {
  if (pinNumber >= 100) {
    pinNumber -= 100;
    return (gpio_pin_get_dt(arduino_pins[pinNumber]) == 1) ? HIGH : LOW;
  }
  return (gpio_pin_get_dt(arduino_pins[pinNumber]) == 1) ? HIGH : LOW;
}

void delay(unsigned long ms) { k_sleep(K_MSEC(ms)); }

void delayMicroseconds(unsigned int us) { k_sleep(K_USEC(us)); }