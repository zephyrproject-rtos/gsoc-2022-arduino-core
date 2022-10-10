/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <Arduino.h>
#include <api/HardwareI2C.h>
#include <api/Print.h>
#include <zephyr/sys/ring_buffer.h>

typedef void (*voidFuncPtrParamInt)(int);

namespace arduino {

class ZephyrI2C : public HardwareI2C {
public:
  ZephyrI2C(const struct device* i2c);

  virtual void begin();
  virtual void end();
  virtual void begin(uint8_t address);
  virtual void setClock(uint32_t freq);

  virtual void beginTransmission(uint8_t address);
  virtual uint8_t endTransmission(bool stopBit);
  virtual uint8_t endTransmission(void);

  virtual size_t requestFrom(uint8_t address, size_t len, bool stopBit);
  virtual size_t requestFrom(uint8_t address, size_t len);

  virtual void onReceive(void (*)(int));
  virtual void onRequest(void (*)(void));

  virtual size_t write(uint8_t data);
  virtual size_t write(int data) { return write((uint8_t)data); };
  virtual size_t write(const uint8_t *buffer, size_t size);
  using Print::write;
  virtual int read();
  virtual int peek();
  virtual void flush();
  virtual int available();

private:
  int _address;
  uint8_t txBuffer[256];
  uint32_t usedTxBuffer;
  struct rx_ring_buf{
    struct ring_buf rb;
    uint8_t buffer[256];
  };
  struct rx_ring_buf rxRingBuffer;
  const struct device* i2c_dev;
};

} // namespace arduino

#if DT_NODE_HAS_PROP(DT_PATH(zephyr_user), i2cs) &&                                                \
		(DT_PROP_LEN(DT_PATH(zephyr_user), i2cs) > 0) ||                                   \
	DT_NODE_EXISTS(DT_NODELABEL(arduino_i2c))
extern arduino::ZephyrI2C Wire;
#endif

typedef arduino::ZephyrI2C TwoWire;
