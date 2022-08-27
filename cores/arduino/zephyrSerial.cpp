/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyrSerial.h>

size_t arduino::ZephyrSerial::begin(unsigned long int baudrate){
    return 0;
}

size_t arduino::ZephyrSerial::print_char(char ch, bool lf){
    printk(lf ? "%c\n" : "%c", ch);
    return lf ? 2 : 1;
}

size_t arduino::ZephyrSerial::print_str(const char* ptr, bool lf) {
    printk(lf ? "%s\n" : "%s", ptr);
    return lf ? strlen(ptr)+1 : strlen(ptr);
}

size_t arduino::ZephyrSerial::print(char ch){
    return print_char(ch, false);
}

size_t arduino::ZephyrSerial::println(char ch){
    return print_char(ch, true);
}

size_t arduino::ZephyrSerial::print(const char* ptr) {
    return print_str(ptr, false);
}

size_t arduino::ZephyrSerial::println(const char* ptr){
    return print_str(ptr, true);
}

size_t arduino::ZephyrSerial::println(void){
    printk("\n");
    return 0;
}

arduino::ZephyrSerial Serial;
