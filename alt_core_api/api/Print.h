/*
 * Copyright (c) 2026 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <print_interface.hpp>

namespace arduino {

class String;
class Printable;

class Print : virtual public PrintInterface {
	class ErrorCode {
		int code = 0;

		void setError(int err) {
			code = err;
		}

		int getError() {
			return code;
		}

		void clearError() {
			setError(0);
		}

		friend class Print;
	} errcode;

protected:
	void setWriteError(int err = 1) {
		errcode.setError(err);
	}

public:
	using PrintInterface::write;

	int getWriteError() {
		return errcode.getError();
	}

	void clearWriteError() {
		errcode.clearError();
	}

	virtual size_t write(const uint8_t *buffer, size_t size);

	size_t write(const char *buffer, size_t size) {
		if (buffer != nullptr && size != 0) {
			return write(reinterpret_cast<const uint8_t *>(buffer), size);
		} else {
			return 0;
		}
	}

	size_t write(const char *str) {
		if (str != nullptr) {
			return write(str, strlen(str));
		} else {
			return 0;
		}
	}

	int availableForWrite() {
		return 0;
	}

	void flush() {
	}

	//TODO size_t print(const String &);
	size_t print(const char[]);
	size_t print(char);
	size_t print(unsigned char, int = 10);
	size_t print(int, int = 10);
	size_t print(unsigned int, int = 10);
	size_t print(long, int = 10);
	size_t print(unsigned long, int = 10);
	size_t print(long long, int = 10);
	size_t print(unsigned long long, int = 10);
	size_t print(double, int = 2);
	//TODO size_t print(const Printable &);

	//TODO size_t println(const String &s);
	size_t println(const char[]);
	size_t println(char);
	size_t println(unsigned char, int = 10);
	size_t println(int, int = 10);
	size_t println(unsigned int, int = 10);
	size_t println(long, int = 10);
	size_t println(unsigned long, int = 10);
	size_t println(long long, int = 10);
	size_t println(unsigned long long, int = 10);
	size_t println(double, int = 2);
	//TODO size_t println(const Printable &);
	size_t println(void);
};

namespace zephyr {

int cbprintf_callback(int c, void *ctx);
size_t wrap_cbprintf(void *ctx, const char *format, ...);
size_t print_number_base_any(void *ctx, unsigned long long ull, int base);
size_t print_number_base_pow2(void *ctx, unsigned long long ull, unsigned bits);

template <class Number>
size_t print_number(void *ctx, Number n, const int base, const char *decfmt) {
	if (base == 0) {
		return reinterpret_cast<Print *>(ctx)->write(static_cast<uint8_t>(n));
	} else if (base == 2) {
		return zephyr::print_number_base_pow2(ctx, n, 1);
	} else if (base == 4) {
		return zephyr::print_number_base_pow2(ctx, n, 2);
	} else if (base == 8) {
		return zephyr::print_number_base_pow2(ctx, n, 3);
	} else if (base == 10) {
		return zephyr::wrap_cbprintf(ctx, decfmt, n);
	} else if (base == 16) {
		return zephyr::print_number_base_pow2(ctx, n, 4);
	} else if (base == 32) {
		return zephyr::print_number_base_pow2(ctx, n, 5);
	} else {
		return zephyr::print_number_base_any(ctx, n, base);
	}
}

} // namespace zephyr

// inline size_t Print::print(const String &s) {
//	return write(s.c_str(), s.length());
// }

inline size_t Print::print(const char str[]) {
	return write(str);
}

inline size_t Print::print(char c) {
	return write(c);
}

inline size_t Print::print(unsigned char n, int base) {
	return zephyr::print_number(this, n, base, "%hhu");
}

inline size_t Print::print(int n, int base) {
	return zephyr::print_number(this, n, base, "%d");
}

inline size_t Print::print(unsigned int n, int base) {
	return zephyr::print_number(this, n, base, "%u");
}

inline size_t Print::print(long n, int base) {
	return zephyr::print_number(this, n, base, "%ld");
}

inline size_t Print::print(unsigned long n, int base) {
	return zephyr::print_number(this, n, base, "%lu");
}

inline size_t Print::print(long long n, int base) {
	return zephyr::print_number(this, n, base, "%lld");
}

inline size_t Print::print(unsigned long long n, int base) {
	return zephyr::print_number(this, n, base, "%llu");
}

inline size_t Print::print(double n, int perception) {
	if (perception < 10) {
		const char ch_perception = static_cast<char>('0' + perception);
		const char format[] = {'%', '.', ch_perception, 'f', '\0'};
		return zephyr::wrap_cbprintf(this, format, n);
	} else {
		const char ch_perception = static_cast<char>('0' + (perception % 10));
		const char format[] = {'%', '.', '1', ch_perception, 'f', '\0'};
		return zephyr::wrap_cbprintf(this, format, n);
	}
}

//TODO inline size_t Print::print(const Printable &printable) {
//	return printable.printTo(*this);
// }

//TODO inline size_t Print::println(const String &s) {
//	return print(s) + println();
//}

inline size_t Print::println(const char str[]) {
	return print(str) + println();
}

inline size_t Print::println(char c) {
	return print(c) + println();
}

inline size_t Print::println(unsigned char uc, int base) {
	return print(uc, base) + println();
}

inline size_t Print::println(int i, int base) {
	return print(i, base) + println();
}

inline size_t Print::println(unsigned int ui, int base) {
	return print(ui, base) + println();
}

inline size_t Print::println(long l, int base) {
	return print(l, base) + println();
}

inline size_t Print::println(unsigned long ul, int base) {
	return print(ul, base) + println();
}

inline size_t Print::println(long long ll, int base) {
	return print(ll, base) + println();
}

inline size_t Print::println(unsigned long long ull, int base) {
	return print(ull, base) + println();
}

inline size_t Print::println(double d, int perception) {
	return print(d, perception) + println();
}

//TODO inline size_t Print::println(const Printable &printable) {
//	return print(printable) + println();
//}

inline size_t Print::println(void) {
	return write("\r\n", 2);
}

/*
 * This is the default implementation.
 * It will be overridden by subclasses.
 */
inline size_t arduino::Print::write(const uint8_t *buffer, size_t size) {
	size_t i;

	for (i = 0; i < size && write(buffer[i]); i++) {
	}

	return i;
}
} // namespace arduino
