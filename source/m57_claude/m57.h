/* Copyright 2021-2022 Alin M Elena <alinm.elena@gmail.com>
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
 * m57.h — keyboard definition header
 * ============================================================
 * Declares the LAYOUT macro that maps the physical key grid to the
 * QMK matrix.  This is the only file that needs to change if the
 * physical wiring of the board changes.
 *
 * Matrix structure:
 *   The m57 is a 5-row × 7-col split keyboard, but QMK sees it as a
 *   single 10-row × 7-col unified matrix:
 *     Rows 0-4  → left half   (L prefix in LAYOUT arguments)
 *     Rows 5-9  → right half  (R prefix in LAYOUT arguments)
 *
 *   Column 6 on several rows has no physical key — those positions
 *   are filled with KC_NO so the matrix array stays rectangular.
 *
 * LAYOUT argument naming:
 *   L<row><col>  — left half key at row 0-4, col 0-6
 *   R<row><col>  — right half key at row 5-9, col 0-6
 *   (col numbering restarts at 0 for the right half in the macro
 *   arguments, even though the matrix stores them in rows 5-9)
 * ============================================================ */

#pragma once

#include "quantum.h"


#define LAYOUT( \
    L00,    L01,    L02,  L03,  L04, L05,                R51,  R52,  R53,  R54 ,R55,  R56, \
    L10,    L11,    L12,  L13,  L14, L15, L16,    R60,   R61,  R62,  R63,  R64, R65,  R66, \
    L20,    L21,    L22,  L23,  L24 ,L25, L26,    R70,   R71,  R72,  R73,  R74, R75,  R76, \
    L30,    L31,    L32,  L33,  L34 ,L35 ,L36,    R80,   R81,  R82,  R83,  R84, R85,  R86, \
                          L43,  L44 ,L45 ,               R91,  R92,  R93 \
) { \
    /* Row 0 — left top row (Esc + number row); col 6 unused → KC_NO */ \
    { L00,    L01,    L02,  L03,  L04, L05, KC_NO }, \
    /* Row 1 — left Q row + rotary encoder key (col 6 = PgUp) */ \
    { L10,    L11,    L12,  L13,  L14, L15, L16 }, \
    /* Row 2 — left home row + encoder key (col 6 = PgDn/Mute) */ \
    { L20,    L21,    L22,  L23,  L24, L25, L26}, \
    /* Row 3 — left Z row; col 6 unused (encoder is mute, no matrix key) */ \
    { L30,    L31,    L32,  L33,  L34, L35, L36}, \
    /* Row 4 — left thumb cluster (3 keys); cols 0-2 and 6 unused → KC_NO */ \
    { KC_NO,  KC_NO,  KC_NO,L43,  L44, L45, KC_NO}, \
    /* Row 5 — right top row; col 0 unused (no key opposite Esc) → KC_NO */ \
    { KC_NO,  R51,    R52,  R53,  R54, R55, R56}, \
    /* Row 6 — right Q row + "{[" key */ \
    { R60,    R61,    R62,  R63,  R64, R65, R66}, \
    /* Row 7 — right home row + "}]" key */ \
    { R70,    R71,    R72,  R73,  R74, R75, R76}, \
    /* Row 8 — right Z row; col 0 unused → KC_NO */ \
    { NO_LED, R81,    R82,  R83,  R84, R85, R86}, \
    /* Row 9 — right thumb cluster; cols 4-6 unused → KC_NO */ \
    { KC_NO,  R91,    R92,  R93,  KC_NO, KC_NO, KC_NO }, \
}



