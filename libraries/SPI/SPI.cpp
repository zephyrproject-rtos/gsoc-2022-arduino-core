/*
 * Copyright (c) 2024 Ayush Singh <ayush@beagleboard.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SPI.h"
#include "zephyrInternal.h"
#include <zephyr/kernel.h>

/* Serial Peripheral Control Register */
uint8_t SPCR;

arduino::ZephyrSPI::ZephyrSPI(const struct device *spi) : spi_dev(spi) {}

uint8_t arduino::ZephyrSPI::transfer(uint8_t data) {
  int ret;
  uint8_t rx;
  const struct spi_buf tx_buf = {.buf = &data, .len = sizeof(data)};
  const struct spi_buf_set tx_buf_set = {
      .buffers = &tx_buf,
      .count = 1,
  };
  const struct spi_buf rx_buf = {.buf = &rx, .len = sizeof(rx)};
  const struct spi_buf_set rx_buf_set = {
      .buffers = &rx_buf,
      .count = 1,
  };

  ret = spi_transceive(spi_dev, &config, &tx_buf_set, &rx_buf_set);
  if (ret < 0) {
    return 0;
  }

  return rx;
}

uint16_t arduino::ZephyrSPI::transfer16(uint16_t data) {
  int ret;
  uint16_t rx;
  const struct spi_buf tx_buf = {.buf = &data, .len = sizeof(data)};
  const struct spi_buf_set tx_buf_set = {
      .buffers = &tx_buf,
      .count = 1,
  };
  const struct spi_buf rx_buf = {.buf = &rx, .len = sizeof(rx)};
  const struct spi_buf_set rx_buf_set = {
      .buffers = &rx_buf,
      .count = 1,
  };

  ret = spi_transceive(spi_dev, &config, &tx_buf_set, &rx_buf_set);
  if (ret < 0) {
    return 0;
  }

  return rx;
}

void arduino::ZephyrSPI::transfer(void *buf, size_t count) {
  int ret;
  const struct spi_buf tx_buf = {.buf = buf, .len = count};
  const struct spi_buf_set tx_buf_set = {
      .buffers = &tx_buf,
      .count = 1,
  };

  ret = spi_write(spi_dev, &config, &tx_buf_set);
  if (ret < 0) {
    return;
  }

  ret = spi_read(spi_dev, &config, &tx_buf_set);
  if (ret < 0) {
    return;
  }
}

void arduino::ZephyrSPI::usingInterrupt(int interruptNumber) {
  interrupt[interrupt_pos++] = interruptNumber;
}

void arduino::ZephyrSPI::notUsingInterrupt(int interruptNumber) {
  for (size_t i = 0; i < interrupt_pos; ++i) {
    if (interrupt[i] == interruptNumber) {
      memmove(&interrupt[i], &interrupt[i + 1], interrupt_pos - i - 1);
      interrupt_pos--;
      break;
    }
  }
}

void arduino::ZephyrSPI::beginTransaction(SPISettings settings) {
  memset(&config, 0, sizeof(config));
  config.frequency = settings.getClockFreq();
  config.operation = ((settings.getBitOrder() ^ 1) << 4) |
                     (settings.getDataMode() << 1) | ((SPCR >> MSTR) & 1) |
                     SPI_WORD_SET(8);

  detachInterrupt();
}

void arduino::ZephyrSPI::endTransaction(void) {
  spi_release(spi_dev, &config);
  attachInterrupt();
}

void arduino::ZephyrSPI::attachInterrupt() {
  for (size_t i = 0; i < interrupt_pos; ++i) {
    enableInterrupt(interrupt[i]);
  }
}

void arduino::ZephyrSPI::detachInterrupt() {
  for (size_t i = 0; i < interrupt_pos; ++i) {
    disableInterrupt(interrupt[i]);
  }
}

void arduino::ZephyrSPI::begin() {}

void arduino::ZephyrSPI::end() {}

arduino::ZephyrSPI
    SPI(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), spis, 0)));
