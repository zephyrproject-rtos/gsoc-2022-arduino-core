/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/sys/time_units.h>

#define clockCyclesPerMicrosecond()  (1000000 / k_cyc_to_ns_near64(1000))
#define clockCyclesToMicroseconds(a) (a / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) (a * clockCyclesPerMicrosecond())
