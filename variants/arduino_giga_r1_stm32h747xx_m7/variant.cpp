/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <stm32_backup_domain.h>

void _on_1200_bps() {
    stm32_backup_domain_enable_access();
    uint32_t tmp = (uint32_t) & (RTC->BKP0R);
    tmp += (RTC_BKP_DR0 * 4U);
    *(__IO uint32_t *)tmp = (uint32_t)0xDF59;
    stm32_backup_domain_disable_access();
    NVIC_SystemReset();
}
