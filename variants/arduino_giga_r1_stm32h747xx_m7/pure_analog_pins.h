/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PURE_ANALOG_PINS_
#define _PURE_ANALOG_PINS_
#ifdef __cplusplus

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "Arduino.h"

/******************************************************************************
 * PREPROCESSOR-MAGIC
 ******************************************************************************/

#define PURE_ANALOG_AS_DIGITAL_ATTRIBUTE __attribute__ ((error("Can't use pins A8-A11 as digital")))

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

class PureAnalogPin {
public:
	PureAnalogPin(int _pin) : pin(_pin) {};
	int get() {
		return pin;
	};
	bool operator== (PureAnalogPin const & other) const {
		return pin == other.pin;
	}
	//operator int() = delete;
	__attribute__ ((error("Change me to a #define"))) operator int();
private:
	int pin;
};

extern PureAnalogPin  A8_PURE;
extern PureAnalogPin  A9_PURE;
extern PureAnalogPin  A10_PURE;
extern PureAnalogPin  A11_PURE;

#define A8  A8_PURE
#define A9  A9_PURE
#define A10  A10_PURE
#define A11  A11_PURE

/******************************************************************************
 * FUNCTION DECLARATION
 ******************************************************************************/

void      PURE_ANALOG_AS_DIGITAL_ATTRIBUTE pinMode     (PureAnalogPin pin, PinMode mode);
PinStatus PURE_ANALOG_AS_DIGITAL_ATTRIBUTE digitalRead (PureAnalogPin pin);
void      PURE_ANALOG_AS_DIGITAL_ATTRIBUTE digitalWrite(PureAnalogPin pin, PinStatus value);
int       analogRead  (PureAnalogPin pin);
void      PURE_ANALOG_AS_DIGITAL_ATTRIBUTE analogWrite (PureAnalogPin pin, int value);

#undef PURE_ANALOG_AS_DIGITAL_ATTRIBUTE

#endif /* __cplusplus */

#endif /* _PURE_ANALOG_PINS_ */
