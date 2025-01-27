/*
 * Copyright (c) 2025 Purva Yeshi <purvayeshi550@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>

#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/sys/reboot.h>

namespace arduino {

class ZephyrEEPROM {
public:
    /* Constructor */
    ZephyrEEPROM() = default;

    /* Initialize the NVS storage (mounts the NVS file system) */
    int nvs_init(void);


    /*
     * Write data to NVS
     * 
     * Parameters:
     *   - id: Unique identifier for the data
     *   - data: Pointer to the data to write
     *   - data_len: Length of the data to write
     */
    int write_data(uint16_t id, const void *data, size_t data_len);


    /* 
     * Read data from NVS
     *
     * Parameters:
     *   - id: Unique identifier for the data
     *   - data: Pointer to buffer where the data will be read into
     *   - max_len: Maximum length of data to read
     */
    int read_data(uint16_t id, void *data, size_t max_len);


private:
    /* NVS file system structure used for managing flash memory */ 
    struct nvs_fs fs;
};

}  

extern arduino::ZephyrEEPROM EEPROM;

    