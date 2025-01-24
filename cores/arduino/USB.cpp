/*
 * Copyright (c) 2024 Arduino SA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/uart/cdc_acm.h>
#include <zephyr/usb/usb_device.h>
#include <SerialUSB.h>

#if (DT_NODE_HAS_PROP(DT_PATH(zephyr_user), cdc_acm) && CONFIG_USB_CDC_ACM)
const struct device *const usb_dev = DEVICE_DT_GET(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), cdc_acm, 0));

void usb_status_cb(enum usb_dc_status_code cb_status, const uint8_t *param) {
    if (cb_status == USB_DC_CONFIGURED) {

    }
}

void __attribute__((weak)) _on_1200_bps() {
    NVIC_SystemReset();
}

void arduino::SerialUSB_::_baudChangeHandler()
{
    uart_line_ctrl_get(uart, UART_LINE_CTRL_BAUD_RATE, &baudrate);
    if (baudrate == 1200) {
        usb_disable();
        _on_1200_bps();
    }
}

static void _baudChangeHandler(const struct device *dev, uint32_t rate)
{
    if (rate == 1200) {
        usb_disable();
        _on_1200_bps();
    }
}

void arduino::SerialUSB_::_baudChangeDispatch(struct k_timer *timer) {
    arduino::SerialUSB_* dev = (arduino::SerialUSB_*)k_timer_user_data_get(timer);
    dev->_baudChangeHandler();
}


void arduino::SerialUSB_::begin(unsigned long baudrate, uint16_t config) {
    if (!started) {
        usb_enable(NULL);
        #ifndef CONFIG_CDC_ACM_DTE_RATE_CALLBACK_SUPPORT
        k_timer_init(&baud_timer, SerialUSB_::_baudChangeDispatch, NULL);
        k_timer_user_data_set(&baud_timer, this);
        k_timer_start(&baud_timer, K_MSEC(100), K_MSEC(100));
        #else
        cdc_acm_dte_rate_callback_set(usb_dev, ::_baudChangeHandler);
        #endif
        ZephyrSerial::begin(baudrate, config);
        started = true;
    }
}

arduino::SerialUSB_::operator bool() {
    uart_line_ctrl_get(uart, UART_LINE_CTRL_DTR, &dtr);
    return dtr;
}

arduino::SerialUSB_ Serial(usb_dev);
#endif