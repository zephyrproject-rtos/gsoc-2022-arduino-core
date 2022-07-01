/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Wire.h"

void arduino::ZephyrI2C::begin() {
  i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
  ring_buf_init(&rxRingBuffer.rb, sizeof(rxRingBuffer.buffer), rxRingBuffer.buffer);
}

void arduino::ZephyrI2C::begin(uint8_t slaveAddr) {

}

void arduino::ZephyrI2C::end() {}

void arduino::ZephyrI2C::setClock(uint32_t freq) {}

void arduino::ZephyrI2C::beginTransmission(uint8_t address) { // TODO for ADS1115
  _address = address;
  usedTxBuffer = 0;
}

uint8_t arduino::ZephyrI2C::endTransmission(bool stopBit) {
  int ret = i2c_write(i2c_dev, txBuffer, usedTxBuffer, _address);
  if (ret) {
    return 1; // fail
  }
  return 0;
}

uint8_t arduino::ZephyrI2C::endTransmission(void) { // TODO for ADS1115
  return endTransmission(true);
}

size_t arduino::ZephyrI2C::requestFrom(uint8_t address, size_t len,
                                       bool stopBit) {
  int ret = i2c_read(i2c_dev, rxRingBuffer.buffer, len, address);
  if (ret != 0)
  {
    printk("\n\nERR: i2c burst read fails\n\n\n");
    return 0;
  }
  ret = ring_buf_put(&rxRingBuffer.rb, rxRingBuffer.buffer, len);
  if (ret == 0)
  {
    printk("\n\nERR: buff put fails\n\n\n");
    return 0;
  }
  return len;
}

size_t arduino::ZephyrI2C::requestFrom(uint8_t address, size_t len) { // TODO for ADS1115
  return requestFrom(address, len, true);
}

size_t arduino::ZephyrI2C::write(uint8_t data) {  // TODO for ADS1115
  txBuffer[usedTxBuffer++] = data;
  return 1;
}

size_t arduino::ZephyrI2C::write(const uint8_t *buffer, size_t size) {
  return size;
}

int arduino::ZephyrI2C::read() {
  uint8_t buf[1];
  if (ring_buf_peek(&rxRingBuffer.rb, buf, 1) > 0) {
        int ret = ring_buf_get(&rxRingBuffer.rb, buf, 1);
        if (ret == 0) {
          printk("\n\nERR: buff empty\n\n\n");
            return 0;
        }
		return (int)buf[0];
  }
  return rxBuffer[0];
}

int arduino::ZephyrI2C::available() { // TODO for ADS1115 
  return 1; 
  }

int arduino::ZephyrI2C::peek() { return 1; }

void arduino::ZephyrI2C::flush() {}

void arduino::ZephyrI2C::onReceive(voidFuncPtrParamInt cb) {}
void arduino::ZephyrI2C::onRequest(voidFuncPtr cb) {}

arduino::ZephyrI2C Wire;