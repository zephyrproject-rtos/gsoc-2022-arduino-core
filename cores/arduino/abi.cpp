/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>

extern "C" {

void *__dso_handle = (void *)&__dso_handle;

void __cxa_pure_virtual(void) {
}

void __cxa_deleted_virtual(void) {
}

int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle) {
	(void)func;
	(void)arg;
	(void)dso_handle; // unused
	return 0;
}

} /* extern "C" */
