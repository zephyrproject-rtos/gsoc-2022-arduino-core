/*
 * Copyright 2025 Arduino SA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/devicetree.h>
#include "wiced_resource.h"

#define QUADSPI_MMAP_BASE   DT_REG_ADDR_BY_IDX(DT_NODELABEL(quadspi), 1)
#define FLASH_CHIP_OFFSET   DT_REG_ADDR(DT_NODELABEL(qspi_flash))
#define AIROC_PART_OFS      DT_REG_ADDR(DT_NODELABEL(airoc_firmware))

#define FW_ADDR (QUADSPI_MMAP_BASE + FLASH_CHIP_OFFSET + AIROC_PART_OFS)
#define FW_SIZE 421098 /* same as the _bin.c file */

const resource_hnd_t wifi_firmware_image = {
	RESOURCE_IN_MEMORY,
	FW_SIZE,
	{ .mem = { (const char *) FW_ADDR } }
};
