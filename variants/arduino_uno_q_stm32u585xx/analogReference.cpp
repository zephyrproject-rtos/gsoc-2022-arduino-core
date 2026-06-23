/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

extern "C" {
    int analog_reference(uint8_t mode);
};

void analogReference(uint8_t mode)
{
  analog_reference(mode);
}