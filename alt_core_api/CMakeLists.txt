# SPDX-License-Identifier: Apache-2.0

function(rust_target_arch RUST_TARGET)
  # Map Zephyr targets to LLVM targets.
  if(CONFIG_CPU_CORTEX_M)
    if(CONFIG_CPU_CORTEX_M0 OR CONFIG_CPU_CORTEX_M0PLUS OR CONFIG_CPU_CORTEX_M1)
      set(${RUST_TARGET} "thumbv6m-none-eabi" PARENT_SCOPE)
    elseif(CONFIG_CPU_CORTEX_M3)
      set(${RUST_TARGET} "thumbv7m-none-eabi" PARENT_SCOPE)
    elseif(CONFIG_CPU_CORTEX_M4 OR CONFIG_CPU_CORTEX_M7)
      if(CONFIG_FP_HARDABI OR FORCE_FP_HARDABI)
        set(${RUST_TARGET} "thumbv7em-none-eabihf" PARENT_SCOPE)
      else()
        set(${RUST_TARGET} "thumbv7em-none-eabi" PARENT_SCOPE)
      endif()
    elseif(CONFIG_CPU_CORTEX_M23)
      set(${RUST_TARGET} "thumbv8m.base-none-eabi" PARENT_SCOPE)
    elseif(CONFIG_CPU_CORTEX_M33 OR CONFIG_CPU_CORTEX_M55)
      if(CONFIG_FP_HARDABI OR FORCE_FP_HARDABI)
        set(${RUST_TARGET} "thumbv8m.main-none-eabihf" PARENT_SCOPE)
      else()
        set(${RUST_TARGET} "thumbv8m.main-none-eabi" PARENT_SCOPE)
      endif()
    else()
      message(FATAL_ERROR "Unknown Cortex-M target.")
    endif()
  elseif(CONFIG_RISCV)
    if(CONFIG_RISCV_ISA_RV64I)
      set(${RUST_TARGET} "riscv64imac-unknown-none-elf" PARENT_SCOPE)
    elseif(CONFIG_RISCV_ISA_RV32I)
      set(${RUST_TARGET} "riscv32i-unknown-none-elf" PARENT_SCOPE)
    else()
      message(FATAL_ERROR "Rust: Unsupported riscv ISA")
    endif()
  elseif(CONFIG_ARCH_POSIX AND CONFIG_64BIT AND (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "x86_64"))
    set(${RUST_TARGET} "x86_64-unknown-none" PARENT_SCOPE)
  elseif(CONFIG_ARCH_POSIX AND CONFIG_64BIT AND (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "aarch64"))
    set(${RUST_TARGET} "aarch64-unknown-none" PARENT_SCOPE)
  else()
    message(FATAL_ERROR "Rust: Add support for other target")
  endif()
endfunction()

rust_target_arch(RUST_TARGET_TRIPLE)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/rust/Cargo.toml)
  set(RUST_CRATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rust)
else()
  set(RUST_CRATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../rust)
endif()
set(RUST_OUT_DIR   ${CMAKE_CURRENT_BINARY_DIR}/rust)
set(RUST_LIB       ${RUST_OUT_DIR}/target/${RUST_TARGET_TRIPLE}/release/libarduinocore_api_rust.a)
set(RUST_SRCS
  ${RUST_CRATE_DIR}/src/lib.rs
  ${RUST_CRATE_DIR}/src/common.rs
)

add_custom_command(
  OUTPUT ${RUST_LIB}
  COMMAND ${CMAKE_COMMAND} -E env CARGO_TARGET_DIR=${RUST_OUT_DIR}/target
    cargo build --manifest-path ${RUST_CRATE_DIR}/Cargo.toml
    --target ${RUST_TARGET_TRIPLE} --release
  WORKING_DIRECTORY ${RUST_CRATE_DIR}
  DEPENDS ${RUST_CRATE_DIR}/Cargo.toml ${RUST_SRCS}
  COMMENT "Building Rust staticlib for ${RUST_TARGET_TRIPLE}"
  VERBATIM
)

add_custom_target(arduinocore_api_rust_build ALL DEPENDS ${RUST_LIB})
add_library(arduinocore_api_rust STATIC IMPORTED GLOBAL)
set_target_properties(arduinocore_api_rust PROPERTIES IMPORTED_LOCATION ${RUST_LIB})
zephyr_link_libraries(arduinocore_api_rust)

add_subdirectory(idl)
add_subdirectory(api)

if(TARGET arduinocore_idl_generate_interfaces)
  add_dependencies(app arduinocore_idl_generate_interfaces)
endif()
