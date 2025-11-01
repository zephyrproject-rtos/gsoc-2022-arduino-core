// Copyright (c) 2025 TOKITA Hiroshi
// SPDX-License-Identifier: Apache-2.0

#![no_std]

mod common;
pub use common::*;

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}
