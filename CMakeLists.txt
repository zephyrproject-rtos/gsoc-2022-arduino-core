# SPDX-License-Identifier: Apache-2.0

if (CONFIG_ARDUINO_API)
  string(REPLACE "__" "_" NORMALIZED_BOARD_TARGET "${NORMALIZED_BOARD_TARGET}")

  # Find variant directory and check for variant.h
  # If board-specific variant.h exists, use it; otherwise fall back to default
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/variant.h)
    set(variant_dir variants/${BOARD})
  elseif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/variants/${NORMALIZED_BOARD_TARGET}/variant.h)
    set(variant_dir variants/${NORMALIZED_BOARD_TARGET})
  else()
    set(variant_dir variants/default)
  endif()

  add_subdirectory(cores)
  add_subdirectory(libraries)
  zephyr_include_directories(${variant_dir})

  if (CONFIG_USE_ARDUINO_API_RUST_IMPLEMENTATION)
    zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/alt_core_api)
    add_subdirectory(alt_core_api)
  else()
    zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/)
    zephyr_sources(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/api/CanMsg.cpp)
    zephyr_sources(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/api/CanMsgRingbuffer.cpp)
    zephyr_sources(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/api/Common.cpp)
    zephyr_sources(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/api/IPAddress.cpp)
    zephyr_sources(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/api/Print.cpp)
    zephyr_sources(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/api/Stream.cpp)
    zephyr_sources(${CMAKE_CURRENT_SOURCE_DIR}/zephyr/blobs/ArduinoCore-API/api/String.cpp)
  endif()

endif()
