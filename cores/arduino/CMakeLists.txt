# SPDX-License-Identifier: Apache-2.0
zephyr_include_directories(.)

if(NOT DEFINED ARDUINO_BUILD_PATH)

zephyr_sources(Print.cpp)
zephyr_sources(Common.cpp)

endif()

