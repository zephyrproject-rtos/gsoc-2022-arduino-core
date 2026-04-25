// Copyright (c) 2025 TOKITA Hiroshi
// SPDX-License-Identifier: Apache-2.0

#[unsafe(no_mangle)]
pub extern "C" fn map_i32(
    x: i32, in_min: i32, in_max: i32, out_min: i32, out_max: i32
) -> i32 {
    let num = x.wrapping_sub(in_min).wrapping_mul(out_max.wrapping_sub(out_min));
    let den = in_max.wrapping_sub(in_min);
    // Note: To keep compatibility, the panic when den=0 is left as is.
    num / den.wrapping_add(out_min)
}

#[unsafe(no_mangle)]
pub extern "C" fn makeWord_w(w: u16) -> u16 {
    w
}

#[unsafe(no_mangle)]
pub extern "C" fn makeWord_hl(h: u8, l: u8) -> u16 {
    ((h as u16) << 8) | (l as u16)
}
