/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <api/itoa.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <api/deprecated-avr-comp/avr/dtostrf.c.impl>

extern char *itoa(int value, char *string, int radix) {
	return ltoa(value, string, radix);
}

extern char *ltoa(long value, char *string, int radix) {
	char tmp[33];
	char *tp = tmp;
	long i;
	unsigned long v;
	int sign;
	char *sp;

	if (string == NULL) {
		return 0;
	}

	if (radix > 36 || radix <= 1) {
		return 0;
	}

	sign = (radix == 10 && value < 0);
	if (sign) {
		v = -value;
	} else {
		v = (unsigned long)value;
	}

	while (v || tp == tmp) {
		i = v % radix;
		v = v / radix;
		if (i < 10) {
			*tp++ = i + '0';
		} else {
			*tp++ = i + 'a' - 10;
		}
	}

	sp = string;

	if (sign) {
		*sp++ = '-';
	}
	while (tp > tmp) {
		*sp++ = *--tp;
	}
	*sp = 0;

	return string;
}

extern char *utoa(unsigned int value, char *string, int radix) {
	return ultoa(value, string, radix);
}

extern char *ultoa(unsigned long value, char *string, int radix) {
	char tmp[33];
	char *tp = tmp;
	long i;
	unsigned long v = value;
	char *sp;

	if (string == NULL) {
		return 0;
	}

	if (radix > 36 || radix <= 1) {
		return 0;
	}

	while (v || tp == tmp) {
		i = v % radix;
		v = v / radix;
		if (i < 10) {
			*tp++ = i + '0';
		} else {
			*tp++ = i + 'a' - 10;
		}
	}

	sp = string;

	while (tp > tmp) {
		*sp++ = *--tp;
	}
	*sp = 0;

	return string;
}

#ifdef __cplusplus
} // extern "C"
#endif
