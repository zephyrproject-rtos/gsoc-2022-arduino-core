/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Common.h"
#include "kernel.h"
#include <ArduinoAPI.h>

void setup();
void loop();

int main()
{
		setup();
		while (1) {
		loop();
		}
		return 0;
}
void setup()
{
	printk("Hello World! %s\n", CONFIG_BOARD);
}
void loop()
{
		printk("Inside loop!\n");
		delay(1000);	// 1 second delay
}
