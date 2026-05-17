#pragma once

/* Vial keyboard identifier — uniquely identifies this board in the Vial app.
 * Do NOT change unless creating a new, distinct board definition. */
#define VIAL_KEYBOARD_UID {0x45, 0x89, 0xD8, 0xFA, 0xC7, 0x2A, 0x36, 0x89}


/* ============================================================
 * VIAL RUNTIME EDITOR SLOT COUNTS
 * ============================================================
 * These control how many entries each Vial live-editor can hold.
 * Slots are stored in EEPROM — increasing counts costs EEPROM space.
 * Decrease if approaching the DYNAMIC_KEYMAP_EEPROM_MAX_ADDR limit (4095 bytes). */

/* Tap-Dance slots in Vial's Tap-Dance editor.
 * Each slot has four configurable actions: tap / hold / double-tap / tap-hold. */
#define VIAL_TAP_DANCE_ENTRIES 8

/* Combo slots in Vial's Combo editor (runtime, stored in EEPROM).
 * These are independent of and coexist with COMBO_COUNT hardcoded combos. */
#define VIAL_COMBO_ENTRIES 8

/* Key Override slots in Vial's Key Override editor.
 * Examples: Shift+Backspace → Delete; Shift+Esc → ~; Ctrl+Z → Ctrl+Shift+Z. */
#define VIAL_KEY_OVERRIDE_ENTRIES 8


/* ============================================================
 * UNICODE INPUT METHODS
 * ============================================================
 * Defines which OS input methods the UC() cycle includes.
 * The active method is set automatically at boot from the stored OS type —
 * see sync_unicode_mode() in keymap.c.  No manual cycling needed.
 *
 * Platform details (no extra software required):
 *   UNICODE_MODE_WIN    — Alt+numpad codes        (Windows, built-in)
 *   UNICODE_MODE_MACOS  — Unicode Hex Input        (Mac; enable once: System Prefs → Keyboard → Input Sources)
 *   UNICODE_MODE_LINUX  — Ctrl+Shift+U             (Linux/Android, built-in) */
#define UNICODE_SELECTED_MODES UNICODE_MODE_WIN, UNICODE_MODE_MACOS, UNICODE_MODE_LINUX


/* ============================================================
 * TAP / HOLD TIMING
 * ============================================================ */

/* Time (ms) to distinguish a tap from a hold.  Used by Mod-Tap (MT), Layer-Tap (LT),
 * and Tap-Dance.  Tune live without reflashing: assign DT_UP / DT_DOWN in Vial.
 * Typical range: 150–250 ms.  Lower = snappier; higher = more forgiving for slow typists. */
#define TAPPING_TERM 200

/* How long (ms) a One-Shot mod or layer waits for the next keypress before it
 * self-cancels.  No Vial panel — change here and reflash to tune. */
#define ONE_SHOT_TIMEOUT 1000

/* Double-tap KC_LSFT (either Shift key) to activate Caps Word instead of the
 * standard "press both Shifts" method.  Required here because only one Shift key
 * (KC_LSFT) is present on the BASE layer — the two-Shift method would never fire. */
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD


/* ============================================================
 * MOUSE KEY SPEED
 * ============================================================
 * No Vial panel for these values — edit here and reflash to tune.
 *
 * Without any KC_MS_ACCEL* held, movement starts at MOVE_DELTA px/step
 * and accelerates up to MAX_SPEED × MOVE_DELTA over TIME_TO_MAX steps.
 *   KC_MS_ACCEL0 = slowest (sniper mode)
 *   KC_MS_ACCEL1 = medium
 *   KC_MS_ACCEL2 = fastest
 *
 *  Define                Default   Effect
 *  MOUSEKEY_MOVE_DELTA       8     pixels per step at minimum speed
 *  MOUSEKEY_MAX_SPEED       10     multiplier ceiling  (10 × 8 = 80 px/step max)
 *  MOUSEKEY_TIME_TO_MAX     30     steps to reach max speed (~30 × 16 ms = 480 ms)
 *  MOUSEKEY_INTERVAL        16     ms between movement steps (~60 Hz) */
#define MOUSEKEY_MOVE_DELTA   8
#define MOUSEKEY_MAX_SPEED   10
#define MOUSEKEY_TIME_TO_MAX 30
#define MOUSEKEY_INTERVAL    16