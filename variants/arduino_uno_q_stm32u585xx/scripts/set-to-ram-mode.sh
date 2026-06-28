#!/bin/sh

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

# This script sets the micro of the Arduino UNO Q in RAM mode, and allow to flash it
# with menu option flash=ram.
# This is necessary because a flash in RAM mode is possible only if the micro
# isn't running any other sketch (in flash mode).
# The RAM mode is volatile, so after a power cycle the micro will not execute
# the sketch anymore.
# This is used by the arduino-app-cli when uploading sketches to the Arduino UNO Q.

# Requirements:
#   - arduino-cli
#   - jq

# Usage:
#   ./set-to-ram-mode.sh [port]


tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

sketch_path="${tmpdir}/empty"
mkdir ${sketch_path}

bin_path="${sketch_path}/empty.ino.elf-zsk.bin"
dd if=/dev/zero of=${bin_path} bs=50 count=1 > /dev/null 2>&1

if [ -z "$1" ]; then
  port=$(arduino-cli board list --format json | jq -r '.detected_ports[] | select(.matching_boards[]?.fqbn == "arduino:zephyr:unoq") | .port.address')
else
  port="$1"
fi

arduino-cli upload -p ${port} --input-dir ${sketch_path} -b arduino:zephyr:unoq
