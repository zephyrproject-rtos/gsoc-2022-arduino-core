# SPDX-License-Identifier: Apache-2.0
zephyr_include_directories(.)

if(NOT DEFINED ARDUINO_BUILD_PATH)

zephyr_sources_ifdef(CONFIG_I2C Wire.cpp)

endif()
