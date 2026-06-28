#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

set -e

source venv/bin/activate

# Get the variant name (NORMALIZED_BOARD_TARGET in Zephyr)
variant=$(cmake "-DBOARD=$1" -P extra/get_variant_name.cmake 2>/dev/null | grep 'VARIANT=' | cut -d '=' -f 2)
echo $variant
