/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"

void func() {
	printf("DO NOT MERGE!\n");
	return FALSE;
}

int main(void) {
	func();
  setup();

  for (;;) {
    loop();
  }

  return 0;
}
