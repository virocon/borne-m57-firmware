/* Copyright 2020 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ============================================================
 * mcuconf.h — STM32F401 clock and peripheral configuration
 * ============================================================
 * Overrides QMK's default ChibiOS MCU config for the m57's STM32F401.
 * Uses include_next so only the deltas from the QMK base are defined here.
 *
 * Clock target: 48 MHz system clock from 16 MHz HSE via PLL.
 *   HSE (16 MHz) → PLL (/M=16 × N=192 / P=4) → 48 MHz SYSCLK
 *   USB requires exactly 48 MHz; this PLL config satisfies that.
 *   APB1 (/4 = 12 MHz) and APB2 (/2 = 24 MHz) are within F401 spec.
 *
 * Peripherals enabled:
 *   TIM3    — WS2812 RGB LED PWM output (channel 2, pin A7)
 *   USART1  — split keyboard half-to-half transport (TX=A9, RX=A10)
 * ============================================================ */

#pragma once

#include_next <mcuconf.h>


/* ============================================================
 * PERIPHERAL ENABLES
 * ============================================================ */

/* TIM3: drives WS2812 RGB LEDs via PWM on channel 2 (PA7, alt function 2).
 * DMA1 stream 2 channel 5 handles the bit stream so the CPU is not blocked. */
#undef  STM32_PWM_USE_TIM3
#define STM32_PWM_USE_TIM3  TRUE

/* USART1: full-duplex split transport at 19200 baud (TX=A9, RX=A10, alt function 7). */
#undef  STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1  TRUE


/* ============================================================
 * CLOCK TREE  (STM32F401, 16 MHz HSE → 48 MHz SYSCLK)
 * ============================================================ */

#undef  STM32_NO_INIT
#undef  STM32_HSI_ENABLED
#undef  STM32_LSI_ENABLED
#undef  STM32_HSE_ENABLED
#undef  STM32_LSE_ENABLED
#undef  STM32_CLOCK48_REQUIRED
#undef  STM32_SW
#undef  STM32_PLLSRC
#undef  STM32_PLLM_VALUE
#undef  STM32_PLLN_VALUE
#undef  STM32_PLLP_VALUE
#undef  STM32_PLLQ_VALUE
#undef  STM32_HPRE
#undef  STM32_PPRE1
#undef  STM32_PPRE2
#undef  STM32_RTCSEL
#undef  STM32_RTCPRE_VALUE
#undef  STM32_MCO1SEL
#undef  STM32_MCO1PRE
#undef  STM32_MCO2SEL
#undef  STM32_MCO2PRE
#undef  STM32_I2SSRC
#undef  STM32_PLLI2SN_VALUE
#undef  STM32_PLLI2SR_VALUE
#undef  STM32_PVD_ENABLE
#undef  STM32_PLS
#undef  STM32_BKPRAM_ENABLE

#define STM32_NO_INIT                       FALSE
#define STM32_HSI_ENABLED                   TRUE   /* 16 MHz internal RC — kept on as PLL fallback. */
#define STM32_LSI_ENABLED                   TRUE   /* 32 kHz internal RC — used for RTC/watchdog. */
#define STM32_HSE_ENABLED                   TRUE   /* 16 MHz external crystal — primary clock source. */
#define STM32_LSE_ENABLED                   FALSE  /* 32.768 kHz external RTC crystal — not populated. */
#define STM32_CLOCK48_REQUIRED              TRUE   /* USB requires exactly 48 MHz. */
#define STM32_SW                            STM32_SW_PLL        /* SYSCLK source = PLL output. */
#define STM32_PLLSRC                        STM32_PLLSRC_HSE    /* PLL input = HSE (16 MHz). */
#define STM32_PLLM_VALUE                    16     /* PLL /M: 16 MHz / 16 = 1 MHz VCO input. */
#define STM32_PLLN_VALUE                    192    /* PLL ×N: 1 MHz × 192 = 192 MHz VCO. */
#define STM32_PLLP_VALUE                    4      /* PLL /P: 192 MHz / 4 = 48 MHz SYSCLK. */
#define STM32_PLLQ_VALUE                    4      /* PLL /Q: 192 MHz / 4 = 48 MHz USB clock. */
#define STM32_HPRE                          STM32_HPRE_DIV1     /* AHB = SYSCLK / 1 = 48 MHz. */
#define STM32_PPRE1                         STM32_PPRE1_DIV4    /* APB1 = 48 / 4 = 12 MHz (max 42). */
#define STM32_PPRE2                         STM32_PPRE2_DIV2    /* APB2 = 48 / 2 = 24 MHz (max 84). */
#define STM32_RTCSEL                        STM32_RTCSEL_LSI
#define STM32_RTCPRE_VALUE                  8
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI
#define STM32_MCO1PRE                       STM32_MCO1PRE_DIV1
#define STM32_MCO2SEL                       STM32_MCO2SEL_SYSCLK
#define STM32_MCO2PRE                       STM32_MCO2PRE_DIV5
#define STM32_I2SSRC                        STM32_I2SSRC_CKIN
#define STM32_PLLI2SN_VALUE                 192
#define STM32_PLLI2SR_VALUE                 5
#define STM32_PVD_ENABLE                    FALSE
#define STM32_PLS                           STM32_PLS_LEV0
#define STM32_BKPRAM_ENABLE                 FALSE
