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
 * leftkb/mcuconf.h — left half peripheral overrides
 * ============================================================
 * The left half has no RGB LEDs, so TIM3 (WS2812 PWM driver) is
 * disabled to prevent the PWM peripheral from being initialized
 * unnecessarily.  USART1 is kept enabled for split transport.
 *
 * This file is a reference/vendor artifact — see leftkb/config.h
 * for notes on why the leftkb/ variant is not a build target.
 * ============================================================ */

#pragma once

#include_next <mcuconf.h>

/* TIM3: disabled on left half — no RGB LEDs, no PWM output needed. */
#undef  STM32_PWM_USE_TIM3
#define STM32_PWM_USE_TIM3  FALSE

/* USART1: enabled — split transport to right (master) half runs over USART1. */
#undef  STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1  TRUE
