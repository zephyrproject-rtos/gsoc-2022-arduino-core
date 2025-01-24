/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"
#ifdef CONFIG_LLEXT
#include <zephyr/llext/symbol.h>
#endif

int main(void) {
#if (DT_NODE_HAS_PROP(DT_PATH(zephyr_user), cdc_acm) && CONFIG_USB_CDC_ACM)
  Serial.begin(115200);
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
