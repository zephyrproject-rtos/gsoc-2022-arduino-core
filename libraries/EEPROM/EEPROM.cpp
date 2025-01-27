/*
 * Copyright (c) 2024 Purva Yeshi <purvayeshi550@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#include "EEPROM.h"

static struct nvs_fs fs;

#define NVS_PARTITION        storage_partition
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

/* Register a module for logging */
LOG_MODULE_REGISTER(eeprom, LOG_LEVEL_INF);    

int arduino::ZephyrEEPROM::nvs_init(void)
{
  int rc = 0;
  
  struct flash_pages_info info;

  fs.flash_device = NVS_PARTITION_DEVICE;
  if (!device_is_ready(fs.flash_device)) {
    LOG_ERR("Flash device %s is not ready", fs.flash_device->name);
     return -ENODEV;
  }

  fs.offset = NVS_PARTITION_OFFSET;

  rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
  if (rc) {
    LOG_ERR("Unable to get page info");
    return rc;
  }

  fs.sector_size = info.size;
  fs.sector_count = 3U;

  rc = nvs_mount(&fs);
  if (rc) {
    LOG_ERR("Flash Init failed");
    return rc;
  }

  LOG_INF("NVS initialized successfully");
  return 0;
}

int arduino::ZephyrEEPROM::read_data(uint16_t id, void *data, size_t max_len)
{
  /* Correctly pass data and max_len */ 
  int rc = nvs_read(&fs, id, data, max_len); 
  if (rc > 0) {
    /* Data successfully read */
    return rc; 
  } else if (rc == 0) {
    LOG_ERR("Data not found for ID: %d", id);
  } else {
    LOG_ERR("Error reading data for ID: %d, Error: %d", id, rc);
  }
  /* Return the result code, which can indicate an error or success */
  return rc; 
}

int arduino::ZephyrEEPROM::write_data(uint16_t id, const void *data, size_t data_len)
{
  /* Pass data directly */
  int rc = nvs_write(&fs, id, data, data_len); 
  if (rc < 0) {
    LOG_ERR("Error writing data for ID: %d, Error: %d", id, rc);
  }
  return rc;
}

arduino::ZephyrEEPROM EEPROM;
