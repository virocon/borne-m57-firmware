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
 * rightkb/mcuconf.h — right half peripheral overrides
 * ============================================================
 * The right half is the USB master and hosts the WS2812 RGB LEDs.
 * TIM3 is disabled here because the unified binary already enables it
 * via the top-level mcuconf.h — this file would only be active if
 * building the rightkb/ variant explicitly, in which case TIM3
 * should mirror the top-level setting (TRUE).
 *
 * Currently identical to leftkb/mcuconf.h — kept separate so that
 * right-specific overrides can be added without touching left.
 *
 * This file is a reference/vendor artifact — see rightkb/config.h
 * for notes on why the rightkb/ variant is not a build target.
 * ============================================================ */

#pragma once

#include_next <mcuconf.h>

/* TIM3: set FALSE here for symmetry with leftkb variant.
 * In the unified build (top-level mcuconf.h) TIM3 is TRUE — RGB works. */
#undef  STM32_PWM_USE_TIM3
#define STM32_PWM_USE_TIM3  FALSE

/* USART1: enabled — split transport between halves runs over USART1. */
#undef  STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1  TRUE
