# SPDX-License-Identifier: Apache-2.0

if (CONFIG_ARDUINO_API)
add_subdirectory(cores)
add_subdirectory(libraries)
zephyr_include_directories(variants/${BOARD})
endif()

