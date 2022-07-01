/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"

void pinMode(pin_size_t pinNumber, PinMode pinMode) {
  if (pinNumber >= 100) {
    pinNumber -= 100;
    if (pinMode == INPUT || pinMode == INPUT_PULLDOWN) {
      gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio1)), pinNumber,
                         GPIO_INPUT | GPIO_ACTIVE_LOW);
    } else {
      gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio1)), pinNumber,
                         GPIO_OUTPUT);
    }
  } else if (pinNumber < 100) {
    if (pinMode == INPUT || pinMode == INPUT_PULLDOWN) {
      gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), pinNumber,
                         GPIO_INPUT | GPIO_ACTIVE_LOW);
    } else {
      gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), pinNumber,
                         GPIO_OUTPUT);
    }
  }
}

void digitalWrite(pin_size_t pinNumber, PinStatus status) {
  if (pinNumber >= 100) {
    pinNumber -= 100;
    if (status == HIGH) {
      gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio1)), pinNumber,
                   GPIO_ACTIVE_HIGH);
    } else if (status == LOW) {
      gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio1)), pinNumber,
                   GPIO_ACTIVE_LOW);
    }
  } else if (pinNumber < 100) {
    if (status == HIGH) {
      gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), pinNumber,
                   GPIO_ACTIVE_HIGH);
    } else if (status == LOW) {
      gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), pinNumber,
                   GPIO_ACTIVE_LOW);
    }
  }
}

PinStatus digitalRead(pin_size_t pinNumber) {
  if (pinNumber >= 100) {
    pinNumber -= 100;
    return (gpio_pin_get(DEVICE_DT_GET(DT_NODELABEL(gpio1)), pinNumber) == 1)
               ? HIGH
               : LOW;
  }
  return (gpio_pin_get(DEVICE_DT_GET(DT_NODELABEL(gpio0)), pinNumber) == 1)
             ? HIGH
             : LOW;
}

void delay(unsigned long ms) { k_sleep(K_MSEC(ms)); }

void delayMicroseconds(unsigned int us) { k_sleep(K_USEC(us)); }