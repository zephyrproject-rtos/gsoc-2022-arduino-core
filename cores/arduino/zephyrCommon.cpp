/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

void yield(void) {
  k_yield();
}

/*
 *  The ACTIVE_HIGH flag is set so that A low physical
 *  level on the pin will be interpreted as value 0.
 *  A high physical level will be interpreted as value 1
 */
void pinMode(pin_size_t pinNumber, PinMode pinMode) {
  if (pinMode == INPUT) { // input mode
    gpio_pin_configure_dt(arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_ACTIVE_HIGH);
  } else if (pinMode == INPUT_PULLUP) { // input with internal pull-up
    gpio_pin_configure_dt(arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_HIGH);
  } else if (pinMode == INPUT_PULLDOWN) { // input with internal pull-down
    gpio_pin_configure_dt(arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_PULL_DOWN | GPIO_ACTIVE_HIGH);
  } else if (pinMode == OUTPUT) { // output mode
    gpio_pin_configure_dt(arduino_pins[pinNumber],
                          GPIO_OUTPUT_LOW | GPIO_ACTIVE_HIGH);
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

unsigned long micros(void) {
  return k_cyc_to_us_floor32(k_cycle_get_32());
}

unsigned long millis(void) { return k_uptime_get_32(); }

