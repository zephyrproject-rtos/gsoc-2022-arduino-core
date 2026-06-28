# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

# get root dir for the project
cmake_path(SET TOP_DIR NORMALIZE ${CMAKE_CURRENT_LIST_DIR}/..)

# get list of variants to be applied
if(CMAKE_ARGC GREATER 3)
	# cmake -P <script> <variant> ...
	foreach(index RANGE 4 ${CMAKE_ARGC})
		math(EXPR index "${index} - 1")
		list(APPEND VARIANTS "${CMAKE_ARGV${index}}")
	endforeach()
	list(TRANSFORM VARIANTS REPLACE "/$" "")
	list(TRANSFORM VARIANTS REPLACE ".*/" "")
else()
	# build for all valid variants
	file(GLOB VARIANTS RELATIVE ${TOP_DIR}/variants variants/*)
	list(REMOVE_ITEM VARIANTS llext linked)
endif()

foreach(variant ${VARIANTS})
	if(NOT IS_DIRECTORY variants/${variant})
		continue()
	endif()

	cmake_path(SET dir ${TOP_DIR}/variants/${variant} NORMALIZE)
	if(NOT EXISTS ${dir}/llext-edk/cmake.cflags)
		continue()
	endif()

	message(STATUS "Processing ${variant}")
	include(${dir}/llext-edk/cmake.cflags)

	list(TRANSFORM LLEXT_ALL_INCLUDE_CFLAGS REPLACE "-I${dir}" "-iwithprefixbefore")
	list(JOIN LLEXT_ALL_INCLUDE_CFLAGS "\n" EDK_INCLUDES)
	file(WRITE ${dir}/includes.txt "${EDK_INCLUDES}")

	# exclude -imacros entries in platform from the list, make sure no others are present
	list(FILTER LLEXT_BASE_CFLAGS EXCLUDE REGEX "-imacros.*autoconf.h")
	list(FILTER LLEXT_BASE_CFLAGS EXCLUDE REGEX "-imacros.*zephyr_stdint.h")
	set(other_imacros "${LLEXT_BASE_CFLAGS}")
	list(FILTER other_imacros INCLUDE REGEX "-imacros.*")
	if(other_imacros)
		message(FATAL_ERROR "Unexpected -imacros in LLEXT_BASE_CFLAGS: ${other_imacros}")
	endif()

	# exclude other problematic macros shared between C and C++
	list(FILTER LLEXT_BASE_CFLAGS EXCLUDE REGEX "-fdiagnostics-color=always")

	# (temp) generate C++ flags from C flags
	set(LLEXT_BASE_CXXFLAGS ${LLEXT_BASE_CFLAGS})
	list(FILTER LLEXT_BASE_CXXFLAGS EXCLUDE REGEX "-Wno-pointer-sign")
	list(FILTER LLEXT_BASE_CXXFLAGS EXCLUDE REGEX "-Werror=implicit-int")
	list(FILTER LLEXT_BASE_CXXFLAGS EXCLUDE REGEX "-std=c.*")

	# save flag files
	list(JOIN LLEXT_BASE_CFLAGS "\n" EDK_CFLAGS)
	file(WRITE ${dir}/cflags.txt "${EDK_CFLAGS}")


	list(JOIN LLEXT_BASE_CXXFLAGS "\n" EDK_CXXFLAGS)
	file(WRITE ${dir}/cxxflags.txt "${EDK_CXXFLAGS}")
endforeach()
