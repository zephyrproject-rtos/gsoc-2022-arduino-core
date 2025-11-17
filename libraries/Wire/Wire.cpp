/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Wire.h>
#include <zephyr/sys/util_macro.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(arduino_wire, CONFIG_ARDUINO_API_LOG_LEVEL);

arduino::ZephyrI2C::ZephyrI2C(const struct device *i2c) : i2c_dev(i2c) {
}

void arduino::ZephyrI2C::begin() {
	ring_buf_init(&rxRingBuffer.rb, sizeof(rxRingBuffer.buffer), rxRingBuffer.buffer);
	i2c_dev->ops.init(i2c_dev);
}

void arduino::ZephyrI2C::begin(uint8_t slaveAddr) {
}

void arduino::ZephyrI2C::end() {
#ifdef CONFIG_DEVICE_DEINIT_SUPPORT
	if (i2c_dev->ops.deinit) {
		i2c_dev->ops.deinit(i2c_dev);
	}
#endif
}

void arduino::ZephyrI2C::setClock(uint32_t freq) {
}

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

size_t arduino::ZephyrI2C::requestFrom(uint8_t address, size_t len, bool stopBit) {
	/* Use stack-allocated buffer for reading */
	uint8_t readbuff[sizeof(rxRingBuffer.buffer)];
	if (len > sizeof(readbuff)) {
		LOG_ERR("requested read length (%zu) exceeds buffer size (%zu)", len, sizeof(readbuff));
		return 0;
	}

	int ret = i2c_read(i2c_dev, readbuff, len, address);
	if (ret != 0) {
		LOG_ERR("burst read failed");
		return 0;
	}

	/* Flush the receive buffer so another read() call returns the correct data */
	flush();
	ret = ring_buf_put(&rxRingBuffer.rb, readbuff, len);
	if (ret == 0) {
		LOG_ERR("failed to put data in ring buffer");
		return 0;
	}
	return len;
}

size_t arduino::ZephyrI2C::requestFrom(uint8_t address, size_t len) { // TODO for ADS1115
	return requestFrom(address, len, true);
}

size_t arduino::ZephyrI2C::write(uint8_t data) { // TODO for ADS1115
	if (usedTxBuffer >= sizeof(txBuffer)) {
		LOG_ERR("tx buffer is full");
		return 0;
	}
	txBuffer[usedTxBuffer++] = data;
	return 1;
}

size_t arduino::ZephyrI2C::write(const uint8_t *buffer, size_t size) {
	if (usedTxBuffer + size > 256) {
		size = 256 - usedTxBuffer;
	}
	memcpy(txBuffer + usedTxBuffer, buffer, size);
	usedTxBuffer += size;
	return size;
}

int arduino::ZephyrI2C::read() {
	uint8_t buf[1];

	if (!ring_buf_get(&rxRingBuffer.rb, buf, 1)) {
		LOG_ERR("buffer is empty");
		return -1; // no data available
	}

	return (int)buf[0];
}

int arduino::ZephyrI2C::available() {            // TODO for ADS1115
	return !ring_buf_is_empty(&rxRingBuffer.rb); // ret 0 if empty
}

int arduino::ZephyrI2C::peek() {
	uint8_t buf[1];
	if (!ring_buf_peek(&rxRingBuffer.rb, buf, 1)) {
		// No data available
		return -1;
	}
	return (int)buf[0];
}

void arduino::ZephyrI2C::flush() {
	ring_buf_reset(&rxRingBuffer.rb);
}

void arduino::ZephyrI2C::onReceive(voidFuncPtrParamInt cb) {
}

void arduino::ZephyrI2C::onRequest(voidFuncPtr cb) {
}

#if DT_NODE_HAS_PROP(DT_PATH(zephyr_user), i2cs)
#if (DT_PROP_LEN(DT_PATH(zephyr_user), i2cs) > 1)
#define ARDUINO_WIRE_DEFINED_0 1
#define DECL_WIRE_0(n, p, i)   arduino::ZephyrI2C Wire(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(n, p, i)));
#define DECL_WIRE_N(n, p, i) arduino::ZephyrI2C Wire##i(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(n, p, i)));
#define DECLARE_WIRE_N(n, p, i)                                                                    \
	COND_CODE_1(ARDUINO_WIRE_DEFINED_##i, (DECL_WIRE_0(n, p, i)), (DECL_WIRE_N(n, p, i)))

/* Declare Wire, Wire1, Wire2, ... */
DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), i2cs, DECLARE_WIRE_N)

#undef DECLARE_WIRE_N
#undef DECL_WIRE_N
#undef DECL_WIRE_0
#undef ARDUINO_WIRE_DEFINED_0
#else  // PROP_LEN(i2cs) > 1
/* When PROP_LEN(i2cs) == 1, DT_FOREACH_PROP_ELEM work not correctly. */
arduino::ZephyrI2C Wire(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), i2cs, 0)));
#endif // HAS_PORP(i2cs)
/* If i2cs node is not defined, tries to use arduino_i2c */
#elif DT_NODE_EXISTS(DT_NODELABEL(arduino_i2c))
arduino::ZephyrI2C Wire(DEVICE_DT_GET(DT_NODELABEL(arduino_i2c)));
#endif
