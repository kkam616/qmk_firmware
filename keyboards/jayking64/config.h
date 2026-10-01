// Copyright 2026 jay
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* WS2812 언더글로: A6 = TIM3_CH1 (AF1), DMA는 TIM3_UP = DMA1 채널 3 */
#define WS2812_PWM_DRIVER PWMD3
#define WS2812_PWM_CHANNEL 1
#define WS2812_PWM_PAL_MODE 1
#define WS2812_PWM_DMA_STREAM STM32_DMA1_STREAM3
#define WS2812_PWM_DMA_CHANNEL 3
