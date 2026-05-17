/* Copyright 2021 JasonRen(biu)
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
 * rightkb/config.h — right half build variant override
 * ============================================================
 * Defines I_AM_RIGHT so that per-half conditional code in the firmware
 * can distinguish which PCB is being built.
 *
 * NOTE: This subdirectory is a reference/vendor artifact.  The leftkb/
 * and rightkb/ variants are NOT wired up as separate build targets in
 * the current rules.mk — both halves run the same binary and use
 * SPLIT_HAND_PIN (C1) at boot to detect their role.
 *
 * To build a per-half binary explicitly, add a rules.mk to this folder
 * and invoke: make m57/rightkb:via
 * ============================================================ */
#pragma once

#define I_AM_RIGHT
