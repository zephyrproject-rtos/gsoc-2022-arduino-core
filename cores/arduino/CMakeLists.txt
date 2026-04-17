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

zephyr_sources(wiring_analog.cpp)
zephyr_sources(wiring_digital.cpp)
zephyr_sources(wiring_pulse.cpp)
zephyr_sources(Tone.cpp)
zephyr_sources(WMath.cpp)
zephyr_sources(WInterrupts.cpp)

if(DEFINED CONFIG_ARDUINO_ENTRY)
zephyr_sources(main.cpp)
endif()

endif()

