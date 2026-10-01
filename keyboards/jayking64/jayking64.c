// Copyright 2026 jay
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

void keyboard_pre_init_kb(void) {
    /* 크리스털이 없으므로 USB SOF(1ms)에 맞춰 HSI48을 자동 보정(CRS)한다.
     * CRS_CFGR 리셋값의 SYNCSRC가 이미 USB SOF라서 켜기만 하면 된다. */
    RCC->APB1ENR |= RCC_APB1ENR_CRSEN;
    CRS->CR |= CRS_CR_AUTOTRIMEN | CRS_CR_CEN;

    keyboard_pre_init_user();
}
