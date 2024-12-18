/*
 * Copyright (c) 2024 Ayush Singh <ayush@beagleboard.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SPI.h"
#include "zephyrInternal.h"
#include <zephyr/kernel.h>

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

  ret = spi_transceive(spi_dev, &config16, &tx_buf_set, &rx_buf_set);
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

  uint8_t rx[count];
  const struct spi_buf rx_buf = {.buf = &rx, .len = count};
  const struct spi_buf_set rx_buf_set = {
      .buffers = &rx_buf,
      .count = 1,
  };

  spi_transceive(spi_dev, &config, &tx_buf_set, &rx_buf_set);
  memcpy(buf, rx, count);
}

void arduino::ZephyrSPI::usingInterrupt(int interruptNumber) {
}

void arduino::ZephyrSPI::notUsingInterrupt(int interruptNumber) {
}

#ifndef SPI_MIN_CLOCK_FEQUENCY
#define SPI_MIN_CLOCK_FEQUENCY 1000000
#endif

void arduino::ZephyrSPI::beginTransaction(SPISettings settings) {
  memset(&config, 0, sizeof(config));
  memset(&config16, 0, sizeof(config16));
  config.frequency = settings.getClockFreq() > SPI_MIN_CLOCK_FEQUENCY ? settings.getClockFreq() : SPI_MIN_CLOCK_FEQUENCY;
  config16.frequency = config.frequency;

  auto mode = SPI_MODE_CPOL | SPI_MODE_CPHA;
  switch (settings.getDataMode()) {
    case SPI_MODE0:
      mode = 0; break;
    case SPI_MODE1:
      mode = SPI_MODE_CPHA; break;
    case SPI_MODE2:
      mode = SPI_MODE_CPOL; break;
    case SPI_MODE3:
      mode = SPI_MODE_CPOL | SPI_MODE_CPHA; break;
  }
  config.operation = SPI_WORD_SET(8) | (settings.getBitOrder() == MSBFIRST ? SPI_TRANSFER_MSB : SPI_TRANSFER_LSB) | mode;
  config16.operation = SPI_WORD_SET(16) | (settings.getBitOrder() == MSBFIRST ? SPI_TRANSFER_MSB : SPI_TRANSFER_LSB) | mode;
}

void arduino::ZephyrSPI::endTransaction(void) {
  spi_release(spi_dev, &config);
}

void arduino::ZephyrSPI::attachInterrupt() {}

void arduino::ZephyrSPI::detachInterrupt() {}


void arduino::ZephyrSPI::begin() {
  beginTransaction(SPISettings());
  endTransaction();
}

void arduino::ZephyrSPI::end() {}

#if DT_NODE_HAS_PROP(DT_PATH(zephyr_user), spis)
#if (DT_PROP_LEN(DT_PATH(zephyr_user), spis) > 1)
#define ARDUINO_SPI_DEFINED_0 1
#define DECL_SPI_0(n, p, i)   arduino::ZephyrSPI SPI(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(n, p, i)));
#define DECL_SPI_N(n, p, i)   arduino::ZephyrSPI SPI##i(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(n, p, i)));
#define DECLARE_SPI_N(n, p, i)                                                                     \
	COND_CODE_1(ARDUINO_SPI_DEFINED_##i, (DECL_SPI_0(n, p, i)), (DECL_SPI_N(n, p, i)))

/* Declare SPI, SPI1, SPI2, ... */
DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), spis, DECLARE_SPI_N)

#undef DECLARE_SPI_N
#undef DECL_SPI_N
#undef DECL_SPI_0
#undef ARDUINO_SPI_DEFINED_0
#else  // PROP_LEN(spis) > 1
/* When PROP_LEN(spis) == 1, DT_FOREACH_PROP_ELEM work not correctly. */
arduino::ZephyrSPI SPI(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), spis, 0)));
#endif // HAS_PORP(spis)
/* If spis node is not defined, tries to use arduino_spi */
#elif DT_NODE_EXISTS(DT_NODELABEL(arduino_spi))
arduino::ZephyrSPI SPI(DEVICE_DT_GET(DT_NODELABEL(arduino_spi)));
#endif
