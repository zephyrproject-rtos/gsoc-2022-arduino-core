/*
 * Copyright (c) 2024 Arduino SA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>

extern "C" void __cxa_pure_virtual(void) __attribute__ ((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__ ((__noreturn__));

namespace std {
  [[gnu::weak, noreturn]] void terminate() {
    abort();
  }
}

void __cxa_pure_virtual(void) {
  std::terminate();
}

void __cxa_deleted_virtual(void) {
  std::terminate();
}
