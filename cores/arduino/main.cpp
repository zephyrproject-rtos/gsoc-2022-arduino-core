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
#if DT_NODE_HAS_PROP(DT_PATH(zephyr_user), cdc_acm) || DT_NODE_HAS_PROP(DT_PATH(zephyr_user), serials)
  Serial.begin(115200);
#endif
  setup();

  for (;;) {
    loop();
    if (arduino::serialEventRun) arduino::serialEventRun();
  }

  return 0;
}

#ifdef CONFIG_LLEXT
LL_EXTENSION_SYMBOL(main);
#endif