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