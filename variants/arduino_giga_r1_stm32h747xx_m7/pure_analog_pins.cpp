/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "pure_analog_pins.h"

#undef A8
#undef A9
#undef A10
#undef A11

PureAnalogPin  A8_PURE(0);
PureAnalogPin  A9_PURE(1);
PureAnalogPin  A10_PURE(2);
PureAnalogPin  A11_PURE(3);

int getAnalogReadResolution();

int analogRead(PureAnalogPin pin) {
  return ::analogRead(A8 + pin.get());
}
