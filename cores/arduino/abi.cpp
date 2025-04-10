/*
 * Copyright (c) 2024 Arduino SA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>

extern "C" void __cxa_pure_virtual(void) {}
extern "C" void __cxa_deleted_virtual(void) {}
extern "C" int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle) {
  (void)func; (void)arg; (void)dso_handle; // unused
  return 0;
}

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
