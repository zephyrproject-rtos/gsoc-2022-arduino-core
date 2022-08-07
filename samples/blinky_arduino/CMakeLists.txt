# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(DTC_OVERLAY_FILE $ENV{HOME}/zephyrproject/modules/lib/Arduino-Zephyr-API/variants/ARDUINO_NANO33BLE/arduino_nano_33_ble.overlay)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

target_sources(app PRIVATE src/main.cpp)

zephyr_compile_options(-Wno-unused-variable -Wno-comment)