# SPDX-License-Identifier: Apache-2.0

list(APPEND CMAKE_MODULE_PATH ${ZEPHYR_BASE}/modules/nanopb)
include(nanopb)

set(ARDUINO_IDL_PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(ARDUINO_IDL_CLASS_PROTO_FILES
  print.proto
  stream.proto
  hardware_serial.proto
)

set(ARDUINO_IDL_OPTS_PROTO ${ARDUINO_IDL_PROTO_DIR}/arduino_opts.proto)
set(ARDUINO_IDL_SHARED_PROTO_FILES
  common.proto
)

set(ARDUINO_IDL_PLUGIN ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc-gen-arduinoif)
set(ARDUINO_IDL_PLUGIN_DEPS
  ${ARDUINO_IDL_PLUGIN}
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/common.py
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/core.py
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/enum_renderer.py
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/method_spec.py
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/request_context.py
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/service_model.py
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/service_renderer.py
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools/protoc_gen_arduinoif/templates/ifc_header.hpp.j2
)
set(ARDUINO_IDL_IF_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/arduinoif)
set(ARDUINO_IDL_PB2_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/protoc_gen_arduinoif)
set(ARDUINO_IDL_PB2_FILE ${ARDUINO_IDL_PB2_OUTPUT_DIR}/arduino_opts_pb2.py)
set(ARDUINO_IDL_TYPE_HEADERS)
foreach(proto_file IN LISTS ARDUINO_IDL_SHARED_PROTO_FILES)
  get_filename_component(proto_stem ${proto_file} NAME_WE)
  list(APPEND ARDUINO_IDL_TYPE_HEADERS
    ${ARDUINO_IDL_IF_OUTPUT_DIR}/${proto_stem}_types.h)
endforeach()

set(ARDUINO_IDL_CLASS_PROTOS)
set(ARDUINO_IDL_CLASS_PROTO_REL_PATHS)
set(ARDUINO_IDL_IF_HEADERS)
foreach(proto_file IN LISTS ARDUINO_IDL_CLASS_PROTO_FILES)
  get_filename_component(proto_stem ${proto_file} NAME_WE)
  list(APPEND ARDUINO_IDL_CLASS_PROTOS
    ${ARDUINO_IDL_PROTO_DIR}/${proto_file})
  list(APPEND ARDUINO_IDL_CLASS_PROTO_REL_PATHS
    proto/${proto_file})
  list(APPEND ARDUINO_IDL_IF_HEADERS
    ${ARDUINO_IDL_IF_OUTPUT_DIR}/${proto_stem}_interface.hpp)
endforeach()
set(ARDUINO_IDL_PLUGIN_INPUT_PROTOS
  ${ARDUINO_IDL_CLASS_PROTOS}
)
foreach(proto_file IN LISTS ARDUINO_IDL_SHARED_PROTO_FILES)
  list(APPEND ARDUINO_IDL_PLUGIN_INPUT_PROTOS
    ${ARDUINO_IDL_PROTO_DIR}/${proto_file})
endforeach()

zephyr_library_named(arduinocore_idl)

zephyr_nanopb_sources(arduinocore_idl
  proto/google/protobuf/empty.proto
  proto/google/protobuf/wrappers.proto
  proto/arduino_opts.proto
  ${ARDUINO_IDL_CLASS_PROTO_REL_PATHS}
)

add_custom_command(
  OUTPUT ${ARDUINO_IDL_PB2_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${ARDUINO_IDL_PB2_OUTPUT_DIR}
  COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    --python_out=${ARDUINO_IDL_PB2_OUTPUT_DIR}
    --proto_path=${ARDUINO_IDL_PROTO_DIR}
    ${ARDUINO_IDL_OPTS_PROTO}
  DEPENDS
    ${ARDUINO_IDL_OPTS_PROTO}
  COMMENT "Generating arduino_opts_pb2.py for protoc-gen-arduinoif"
  VERBATIM
)

add_custom_command(
  OUTPUT ${ARDUINO_IDL_IF_HEADERS} ${ARDUINO_IDL_TYPE_HEADERS}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${ARDUINO_IDL_IF_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E env
    PROTOC_GEN_ARDUINOIF_PB2=${ARDUINO_IDL_PB2_FILE}
    ${PROTOBUF_PROTOC_EXECUTABLE}
    --plugin=protoc-gen-arduinoif=${ARDUINO_IDL_PLUGIN}
    --arduinoif_out=${ARDUINO_IDL_IF_OUTPUT_DIR}
    --proto_path=${ARDUINO_IDL_PROTO_DIR}
    ${ARDUINO_IDL_PLUGIN_INPUT_PROTOS}
  DEPENDS
    ${ARDUINO_IDL_PLUGIN_INPUT_PROTOS}
    ${ARDUINO_IDL_OPTS_PROTO}
    ${ARDUINO_IDL_PB2_FILE}
    ${ARDUINO_IDL_PLUGIN_DEPS}
  COMMENT "Generating C++ interface headers from Arduino IDL proto files"
  VERBATIM
)

add_custom_target(arduinocore_idl_generate_interfaces ALL DEPENDS
  ${ARDUINO_IDL_IF_HEADERS}
  ${ARDUINO_IDL_TYPE_HEADERS}
)

target_sources(arduinocore_idl PRIVATE
  ${ARDUINO_IDL_IF_HEADERS}
  ${ARDUINO_IDL_TYPE_HEADERS}
)

zephyr_include_directories(${ARDUINO_IDL_IF_OUTPUT_DIR})
