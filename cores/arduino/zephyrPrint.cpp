/*
 * Copyright (c) 2022 TOKITA Hiroshi <tokita.hiroshi@fujitsu.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

/*
 * This is the default implementation.
 * It will be overridden by subclassese.
 */
size_t arduino::Print::write(const uint8_t *buffer, size_t size)
{
  size_t i;
  for (i=0; i<size && write(buffer[i]); i++) {
  }

  return i;
}
