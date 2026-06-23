/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * SPDX-License-Identifier: Apache-2.0
 */

/* This file is included only when the core is used with old IDE/CLI tools that
 * do not support multi-step linking, and ensures all .rodata and .rel sections
 * are merged into one. Newer tools should run 'gen_rodata_ld' and use its
 * output for optimal memory use.
 */

SECTIONS {
    .rodata : {
        *(.rodata .rodata.*)
    }

    .rel : {
        *(.rel .rel.*)
    }
}
