/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <Arduino.h>

namespace arduino {

class ZephyrSerial {
    char pvt_c;

private:
    size_t print_char(const char c, bool lf);
    size_t print_str(const char * ptr, bool lf);

public:
    size_t begin(unsigned long int baudrate);  //TODO

    size_t print(const char c);
    size_t print(const int val);
    size_t print(double d);
    size_t print(const char * ptr);
    size_t print(const int val, const int base);

    size_t println(const char c);
    size_t println(const char* ptr);
    size_t println(void);

};

}   // namespace arduino

extern arduino::ZephyrSerial Serial;
