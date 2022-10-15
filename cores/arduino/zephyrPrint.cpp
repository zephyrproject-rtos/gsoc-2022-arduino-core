/*
 * Copyright (c) 2022 TOKITA Hiroshi <tokita.hiroshi@fujitsu.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/cbprintf.h>

#include <Arduino.h>
#include <api/Print.h>

namespace arduino
{
namespace zephyr
{

int cbprintf_callback(int c, void *ctx)
{
	return reinterpret_cast<arduino::Print *>(ctx)->write((unsigned char)c);
}

size_t wrap_cbprintf(void *ctx, const char *format, ...)
{
	va_list ap;
	int rc;

	va_start(ap, format);
	rc = cbvprintf(reinterpret_cast<cbprintf_cb>(cbprintf_callback), ctx, format, ap);
	va_end(ap);

	return static_cast<size_t>(rc > 0 ? rc : 0);
}

size_t print_number_base_any(void *ctx, unsigned long long ull, int base)
{
	arduino::Print &print = *reinterpret_cast<arduino::Print *>(ctx);
	char string[sizeof(unsigned long long) * 8] = {0};
	size_t digit = 0;
	unsigned value;

	if (base < 2 || base > ('~' - 'A' + 10)) {
		base = 10;
	}

	while (ull != 0) {
		value = ull % base;
		if (value < 10) {
			string[sizeof(string) - digit] = '0' + value;
		} else {
			string[sizeof(string) - digit] = 'A' + (value- 10);
		}

		digit++;
		ull /= base;
	}

	return print.write(string + (sizeof(string) - digit), digit + 1);
}

size_t print_number_base_pow2(void *ctx, unsigned long long ull, unsigned bits)
{
	arduino::Print &print = *reinterpret_cast<arduino::Print *>(ctx);
	const unsigned long long mask = (1 << bits) - 1;
	int digit = (((sizeof(unsigned long long) * 8) + bits) / bits);
	int output_count = -1;
	unsigned value;

	while (digit >= 0) {
		value = (ull & (mask << (digit * bits))) >> (digit * bits);
		if (value != 0 && output_count < 0) {
			output_count = 0;
		}

		if (output_count >= 0) {
			if (value < 10) {
				print.write('0' + value);
			} else {
				print.write('A' + (value- 10));
			}
			output_count++;
		}
		digit--;
	}

	return output_count;
}

} // namespace zephyr
} // namespace arduino

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
