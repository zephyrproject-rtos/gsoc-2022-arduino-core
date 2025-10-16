zephyr_interface_library_named(blobs)

if (CONFIG_BOARD_ARDUINO_PORTENTA_C33)
    set(gen_inc_dir ${ZEPHYR_BINARY_DIR}/misc/generated)
    zephyr_include_directories(${gen_inc_dir})
    set(gen_dir ${gen_inc_dir}/c33_bl_patch)
    generate_inc_file_for_target(
        ${ZEPHYR_CURRENT_LIBRARY}
        ${CMAKE_CURRENT_SOURCE_DIR}/c33_bl.bin
        ${gen_dir}/c33_bl.bin.inc
    )
endif()

if(CONFIG_AIROC_WIFI_CUSTOM)
    zephyr_include_directories(${CMAKE_CURRENT_LIST_DIR}) # for wifi_nvram_image.h
    zephyr_library_sources(4343WA1_clm_blob.c)
    zephyr_library_sources(4343WA1_bin_qspi.c) # use _bin.c for a static build
endif()
