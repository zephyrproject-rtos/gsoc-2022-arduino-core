/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"

extern "C" {

int arduino_main(void) {
  setup();

  for (;;) {
    loop();
    if (arduino::serialEventRun) arduino::serialEventRun();
  }

  return 0;
}

}
