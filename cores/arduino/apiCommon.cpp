/*
 * Copyright (c) 2025 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include "zephyrInternal.h"

extern "C" {
  int32_t map_i32(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
  uint16_t makeWord_w(uint16_t w);
  uint16_t makeWord_hl(byte h, byte l);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return map_i32(x, in_min, in_max, out_min, out_max);
}

uint16_t makeWord(uint16_t w) {
  return makeWord_w(w);
}
uint16_t makeWord(byte h, byte l) {
  return makeWord_hl(h, l);
}
