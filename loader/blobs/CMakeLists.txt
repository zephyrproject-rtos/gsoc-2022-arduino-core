zephyr_interface_library_named(blobs)

if (CONFIG_BOARD_ARDUINO_PORTENTA_C33)
    set(gen_inc_dir ${ZEPHYR_BINARY_DIR}/misc/generated)
    zephyr_include_directories(${gen_inc_dir})
    set(gen_dir ${gen_inc_dir}/c33_bl_patch)
    generate_inc_file_for_target(
        ${ZEPHYR_CURRENT_LIBRARY}
        ${CMAKE_CURRENT_SOURCE_DIR}/blobs/c33_bl.bin
        ${gen_dir}/c33_bl.bin.inc
    )
endif()