/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"
#ifdef CONFIG_LLEXT
#include <zephyr/llext/symbol.h>
#endif

#ifdef CONFIG_MULTITHREADING
void start_static_threads();
#endif

#ifdef CONFIG_GPIO_NRFX
#include <zephyr/dt-bindings/gpio/nordic-nrf-gpio.h>
#endif

int main(void) {
#if (DT_NODE_HAS_PROP(DT_PATH(zephyr_user), cdc_acm) && CONFIG_USB_CDC_ACM)
  Serial.begin(115200);
#endif

#ifdef CONFIG_MULTITHREADING
  start_static_threads();
#endif

#ifdef CONFIG_GPIO_NRFX
  static const struct gpio_dt_spec enable_sensors =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), pin_enable_gpios);
  if (gpio_is_ready_dt(&enable_sensors)) {
      gpio_flags_t flags = enable_sensors.dt_flags | GPIO_OUTPUT_HIGH | GPIO_ACTIVE_HIGH | NRF_GPIO_DRIVE_H0H1;
      Serial.println((uint32_t)flags, HEX);
      
      gpio_pin_configure(enable_sensors.port, enable_sensors.pin, flags);
      gpio_pin_set(enable_sensors.port, enable_sensors.pin, HIGH);
      //Serial.println("### Sensor pin enabled ###");

      delay(500);
  }
#endif
  setup();

  for (;;) {
    loop();
  #if (DT_NODE_HAS_PROP(DT_PATH(zephyr_user), cdc_acm) && CONFIG_USB_CDC_ACM) || DT_NODE_HAS_PROP(DT_PATH(zephyr_user), serials)
    if (arduino::serialEventRun) arduino::serialEventRun();
  #endif
  }

  return 0;
}

#ifdef CONFIG_LLEXT
LL_EXTENSION_SYMBOL(main);
#endif
