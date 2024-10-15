# SPDX-License-Identifier: Apache-2.0


if (CONFIG_ARDUINO_CORE)

function(arduino_sources)
  if (IS_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/variants/${BOARD})
    set(variant_dir variants/${BOARD})
  elseif (IS_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/variants/${BOARD}${NORMALIZED_BOARD_QUALIFIERS})
    set(variant_dir variants/${BOARD}${NORMALIZED_BOARD_QUALIFIERS})
  else()
    message(FATAL_ERROR "Variant dir not found: variants/${BOARD}, variants/${BOARD}${NORMALIZED_BOARD_QUALIFIERS}")
  endif()

  set(ext_name arduino)
  set(ext_bin ${ZEPHYR_BINARY_DIR}/${ext_name}.llext)
  set(ext_inc ${ZEPHYR_BINARY_DIR}/include/generated/${ext_name}_ext.inc)

  set(core_srcs
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/module_export.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/zephyrPrint.cpp
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/zephyrSerial.cpp
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/zephyrCommon.cpp
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/main.cpp
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/String.cpp
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/Stream.cpp
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/Common.cpp
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/Print.cpp
  )

  if (CONFIG_SPI)
    list(APPEND core_srcs ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/libraries/SPI/SPI.cpp)
  endif()

  if (CONFIG_I2C)
    list(APPEND core_srcs ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/libraries/Wire/Wire.cpp)
  endif()

  if (CONFIG_CAN)
    list(APPEND core_srcs ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/CanMsgRingbuffer.cpp
	                  ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/CanMsg.cpp)
  endif()

  if (CONFIG_USB)
    list(APPEND core_srcs ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/PluggableUSB.cpp)
  endif()

  if (CONFIG_NET_IP)
    list(APPEND core_srcs ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/IPAddress.cpp)
  endif()

  add_llext_target(${ext_name}_ext
    OUTPUT  ${ext_bin}
    SOURCES
      ${core_srcs}
      ${ARGV}
  )

  llext_include_directories(${ext_name}_ext
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cores/arduino/api/
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/libraries/SPI/
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/libraries/Wire/
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/${variant_dir}
  )

  generate_inc_file_for_target(app ${ext_bin} ${ext_inc})
endfunction()

  if(DEFINED CONFIG_ARDUINO_ENTRY)
    target_sources(app PRIVATE cores/arduino/main_loader.c)
  endif()

endif()

