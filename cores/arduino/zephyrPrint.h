/*
 * Copyright (c) 2022 TOKITA Hiroshi <tokita.hiroshi@fujitsu.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/sys/cbprintf.h>

#include <Arduino.h>
#include <api/Print.h>

namespace arduino
{
namespace zephyr
{

int cbprintf_callback(int c, void *ctx);
size_t wrap_cbprintf(void *ctx, const char *format, ...);
size_t print_number_base_any(void *ctx, unsigned long long ull, int base);
size_t print_number_base_pow2(void *ctx, unsigned long long ull, unsigned bits);

template <class Number> size_t print_number(void *ctx, Number n, const int base, const char *decfmt)
{
	if (base == 0) {
		return reinterpret_cast<arduino::Print *>(ctx)->write((char)n);
	} else if (base == 2) {
		return arduino::zephyr::print_number_base_pow2(ctx, n, 1);
	} else if (base == 4) {
		return arduino::zephyr::print_number_base_pow2(ctx, n, 2);
	} else if (base == 8) {
		return arduino::zephyr::print_number_base_pow2(ctx, n, 3);
	} else if (base == 10) {
		return arduino::zephyr::wrap_cbprintf(ctx, decfmt, n);
	} else if (base == 16) {
		return arduino::zephyr::print_number_base_pow2(ctx, n, 4);
	} else if (base == 32) {
		return arduino::zephyr::print_number_base_pow2(ctx, n, 5);
	} else {
		return arduino::zephyr::print_number_base_any(ctx, n, base);
	}
}

} // namespace zephyr

} // namespace arduino

inline size_t arduino::Print::print(const __FlashStringHelper *fsh)
{
	return write(reinterpret_cast<const char *>(fsh));
}

inline size_t arduino::Print::print(const String &s)
{
	return write(s.c_str(), s.length());
}

inline size_t arduino::Print::print(const char str[])
{
	return write(str);
}

inline size_t arduino::Print::print(char c)
{
	return write(c);
}

inline size_t arduino::Print::print(unsigned char n, int base)
{
	return arduino::zephyr::print_number(this, n, base, "%hhu");
}

inline size_t arduino::Print::print(int n, int base)
{
	return arduino::zephyr::print_number(this, n, base, "%d");
}

inline size_t arduino::Print::print(unsigned int n, int base)
{
	return arduino::zephyr::print_number(this, n, base, "%u");
}

inline size_t arduino::Print::print(long n, int base)
{
	return arduino::zephyr::print_number(this, n, base, "%ld");
}

inline size_t arduino::Print::print(unsigned long n, int base)
{
	return arduino::zephyr::print_number(this, n, base, "%lu");
}

inline size_t arduino::Print::print(long long n, int base)
{
	return arduino::zephyr::print_number(this, n, base, "%lld");
}

inline size_t arduino::Print::print(unsigned long long n, int base)
{
	return arduino::zephyr::print_number(this, n, base, "%llu");
}

inline size_t arduino::Print::print(double n, int perception)
{
	if (perception < 10) {
		const char ch_perception = static_cast<char>('0' + perception);
		const char format[] = {'%', '.', ch_perception, 'f', '\0'};
		return arduino::zephyr::wrap_cbprintf(this, format, n);
	} else {
		const char ch_perception = static_cast<char>('0' + (perception % 10));
		const char format[] = {'%', '.', '1', ch_perception, 'f', '\0'};
		return arduino::zephyr::wrap_cbprintf(this, format, n);
	}
}

inline size_t arduino::Print::print(const Printable &printable)
{
	return printable.printTo(*this);
}

inline size_t arduino::Print::println(const __FlashStringHelper *fsh)
{
	return print(fsh) + println();
}

inline size_t arduino::Print::println(const String &s)
{
	return print(s) + println();
}

inline size_t arduino::Print::println(const char str[])
{
	return print(str) + println();
}

inline size_t arduino::Print::println(char c)
{
	return print(c) + println();
}

inline size_t arduino::Print::println(unsigned char uc, int base)
{
	return print(uc, base) + println();
}

inline size_t arduino::Print::println(int i, int base)
{
	return print(i, base) + println();
}

inline size_t arduino::Print::println(unsigned int ui, int base)
{
	return print(ui, base) + println();
}

inline size_t arduino::Print::println(long l, int base)
{
	return print(l, base) + println();
}

inline size_t arduino::Print::println(unsigned long ul, int base)
{
	return print(ul, base) + println();
}

inline size_t arduino::Print::println(long long ll, int base)
{
	return print(ll, base) + println();
}

inline size_t arduino::Print::println(unsigned long long ull, int base)
{
	return print(ull, base) + println();
}

inline size_t arduino::Print::println(double d, int perception)
{
	return print(d, perception) + println();
}

inline size_t arduino::Print::println(const Printable &printable)
{
	return print(printable) + println();
}

inline size_t arduino::Print::println(void)
{
	return write("\r\n", 2);
}
