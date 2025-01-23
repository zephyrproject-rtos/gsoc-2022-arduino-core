/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef CONFIG_DAC

void analogWrite(enum dacPins pinNumber, int value);

#endif

// In c++ mode, we also provide analogReadResolution and analogWriteResolution getters
int analogReadResolution();
int analogWriteResolution();
