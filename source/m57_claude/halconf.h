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
 * halconf.h — ChibiOS HAL peripheral enables
 * ============================================================
 * Selects which STM32 hardware peripherals the ChibiOS HAL initializes.
 * Uses include_next so only the m57-specific deltas are listed here.
 *
 * Enabled peripherals:
 *   PWM    — TIM3 CH2 for WS2812 RGB LED output (config in mcuconf.h)
 *   SERIAL — USART1 for split keyboard half-to-half transport (TX=A9, RX=A10)
 * ============================================================ */

#pragma once

/* PWM: required for WS2812 RGB LED driver (PWMD3 / TIM3 channel 2, pin A7). */
#define HAL_USE_PWM     TRUE

/* SERIAL: required for split transport (USART1 full-duplex, SD1 driver). */
#define HAL_USE_SERIAL  TRUE

/* USB receive buffer size in bytes.  256 is sufficient for HID + console traffic.
 * Increase if console output is being dropped at high log verbosity. */
#define SERIAL_USB_BUFFERS_SIZE  256

#include_next <halconf.h>
