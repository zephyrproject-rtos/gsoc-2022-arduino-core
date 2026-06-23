/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/*
 * This file is used to detect issues with the Arduino_RouterBridge library not
 * being detected properly. If this gets selected, either the library is not
 * installed or there is an issue with the library detection.
 */

#ifdef ARDUINO_ARCH_ARDUINOCORE_ZEPHYR

/*
 * The core was probably cloned with the original name in the hardware/ sketch
 * folder, but that becomes part of the FQBN and it messes with library search,
 * so even if the library is installed it is deemed "not compatible".
 * Force the user to rename the core folder to "zephyr" to avoid this issue.
 */

#error                                                                                             \
	"Invalid configuration: please rename your 'ArduinoCore-zephyr' folder inside hardware/ to 'zephyr'."

#else

/*
 * If the folder name is correct, then the actual library is really missing and
 * the user needs to be warned about it.
 */

#error                                                                                             \
	"Please install the Arduino_RouterBridge library from the Library Manager for proper Serial support on this board."

#endif
