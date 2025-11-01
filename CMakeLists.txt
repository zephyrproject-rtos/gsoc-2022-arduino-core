# SPDX-License-Identifier: Apache-2.0

if (CONFIG_ARDUINO_API)
  if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}${NORMALIZED_BOARD_QUALIFIERS})
    set(variant_dir variants/${BOARD}${NORMALIZED_BOARD_QUALIFIERS})
  else()
    message(FATAL_ERROR "Variant dir not found: variants/${BOARD}${NORMALIZED_BOARD_QUALIFIERS}")
  endif()

  add_subdirectory(cores)
  add_subdirectory(libraries)
  zephyr_include_directories(${variant_dir})
endif()
