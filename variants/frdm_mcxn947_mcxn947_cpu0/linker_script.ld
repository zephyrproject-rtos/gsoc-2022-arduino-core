/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

FORCE_COMMON_ALLOCATION

SECTIONS {

    /DISCARD/ : {
        *(.ARM.attributes)
        *(.ARM.attributes.*)
        *(.ARM.exidx)
        *(.ARM.exidx.*)
        *(.ARM.extidx)
        *(.ARM.extidx.*)
        *(.ARM.extab)
        *(.ARM.extab.*)
        *(.comment)
        *(.comment.*)
        *(.llvmbc)
        *(.llvmcmd)
    }

    .text 0x00000000 : ALIGN(4) {
        *(.text)
        *(.stub)
        *(.text*)
        *(.text.*)
        *(.text._*)

        KEEP (*(.init))
        KEEP (*(.ctors))
        KEEP (*(.dtors))
        KEEP (*(.fini))
    }

    .rodata : {
        *(.rodata)
        *(.rodata1)
        *(.rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        *(.bss .bss.* COMMON)
    }

    .exported_sym : {
        KEEP(*(.exported_sym))
        KEEP(*(.exported_sym.*))
    }

    .init_array : {
        KEEP(*(.init_array))
    }

    .symtab : {
        KEEP(*(.symtab))
    }

    .strtab : {
        KEEP(*(.strtab))
    }

    .shstrtab : {
        KEEP(*(.shstrtab))
    }

    .rel : {
        KEEP(*(.rel .rel.*))
    }

    .got : {
        KEEP(*(.got .got.* .got.plt .got.plt*))
    }
}