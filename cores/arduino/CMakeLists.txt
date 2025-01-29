# SPDX-License-Identifier: Apache-2.0

# This line ensures that cores/arduino/zephyr/api is loaded ahead of
# cores/arduino/api.
if(CONFIG_USE_ARDUINO_API_RUST_IMPLEMENTATION)
  zephyr_include_directories(zephyr)
endif()

zephyr_include_directories(.)
zephyr_include_directories(../../variants)

if(NOT DEFINED ARDUINO_BUILD_PATH)

zephyr_sources(zephyrSerial.cpp)
zephyr_sources(zephyrCommon.cpp)

if(CONFIG_USE_ARDUINO_API_RUST_IMPLEMENTATION)
  zephyr_sources(zephyrPrint.cpp)
  zephyr_sources(apiCommon.cpp)
endif()

if(DEFINED CONFIG_ARDUINO_ENTRY)
zephyr_sources(main.cpp)
zephyr_sources(threads.cpp)
endif()

endif()

