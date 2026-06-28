# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

#
# Generate NORMALIZED_BOARD_TARGET from BOARD via the Zephyr build system
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE} COMPONENTS yaml boards)
message(STATUS "VARIANT=${NORMALIZED_BOARD_TARGET}")
