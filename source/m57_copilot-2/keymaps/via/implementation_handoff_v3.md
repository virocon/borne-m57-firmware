# borne m57 — implementation specification (v3)

This document is a complete self-contained specification. No existing files need to be read or copied. An implementor reads this document top to bottom and writes all files from scratch.

---

## Hardware context

Split keyboard, STM32F401 MCU. 10×7 matrix: rows 0–4 are the left half, rows 5–9 are the right half. 58 WS2812 LEDs, 29 per side, driven by PWM on pin A7 via TIM3 channel 2. Two rotary encoders: left on B8/B9 (index 0), right on B4/B6 (index 1), both resolution 2. Encoder push positions (L36, R80) have no LED — they map to NO_LED in the LED config. Split communication is full-duplex USART1 on A9/A10 at 19200 baud. Master is always the right half regardless of which USB is connected (MASTER_RIGHT).

VID 0x6401, PID 0x45D4. Vial UID: bytes 0x45, 0x89, 0xD8, 0xFA, 0xC7, 0x2A, 0x36, 0x89.

EEPROM uses wear-leveling (legacy mode), 4KB logical / 8KB backing. Do not exceed address 4095 for dynamic keymap data.

---

## Files to create

Five files live in `keymaps/via/`. All are created from scratch — none exist in the vendor base for this keyboard variant:

- `rules.mk` — build feature flags
- `config.h` — compile-time defines
- `theme.h` — the entire RGB color system
- `keymap.c` — all firmware logic
- `rgb_matrix_user.inc` — custom RGB effect declarations

The keyboard-level files (`m57.c`, `m57.h`, `info.json`, `config.h`, etc.) already exist and must not be modified.

---

## rules.mk

Enable the following features. Add a short comment on each line explaining its purpose and, where relevant, how it relates to the Vial live editor:

- `VIAL_ENABLE = yes` — enables Vial protocol
- `VIA_ENABLE = yes` — enables VIA protocol (Vial depends on it)
- `ENCODER_MAP_ENABLE = yes` — enables per-layer encoder action tables
- `COMBO_ENABLE = yes` — required even with zero hardcoded combos, so Vial's runtime combo editor works
- `VIAL_INSECURE = yes` — allows Vial to unlock the keyboard without a physical key combo
- `TAP_DANCE_ENABLE = yes` — enables tap-dance; slot count set in config.h, configured live in Vial
- `KEY_OVERRIDE_ENABLE = yes` — enables key overrides; used here for Swedish AltGr characters; also editable live in Vial
- `UNICODE_ENABLE = yes` — enables UC() keycodes; required for Swedish character output
- `AUTO_SHIFT_ENABLE = yes` — hold any key longer than the threshold to get its shifted version; threshold tunable in Vial
- `MOUSEKEY_ENABLE = yes` — enables mouse movement/click/scroll keycodes used in the NAV layer
- `REPEAT_KEY_ENABLE = yes` — enables QK_REPEAT_KEY / QK_ALT_REPEAT_KEY keycodes
- `DYNAMIC_TAPPING_TERM_ENABLE = yes` — allows tapping term to be adjusted live without reflash via DT_UP/DT_DOWN keys
- `VIALRGB_ENABLE = yes` — enables Vial's RGB mode/brightness controls
- `RGB_MATRIX_CUSTOM_USER = yes` — tells QMK to include `rgb_matrix_user.inc` from this keymap directory and compile the custom effect stubs in keymap.c; **without this the `#ifdef RGB_MATRIX_CUSTOM_USER` guard silently drops the stubs and the custom modes disappear from Vial**

Comment out `CONSOLE_ENABLE` (do not enable it for production — it costs ~2 KB flash). Leave the commented line as a reminder for debug builds.

---

## config.h

Use `#pragma once`. Define the following:

**Vial UID** — `VIAL_KEYBOARD_UID` as a byte array literal: 0x45, 0x89, 0xD8, 0xFA, 0xC7, 0x2A, 0x36, 0x89.

**Combo count** — `COMBO_COUNT` = 0. Comment: no compile-time combos; use Vial's combo editor for runtime combos.

**Vial editor slot counts:**
- `VIAL_TAP_DANCE_ENTRIES` = 8
- `VIAL_COMBO_ENTRIES` = 8
- `VIAL_KEY_OVERRIDE_ENTRIES` = 8

**Unicode modes** — `UNICODE_SELECTED_MODES` = `UNICODE_MODE_WIN, UNICODE_MODE_MACOS, UNICODE_MODE_LINUX`. This populates the cycle list; the active mode is set explicitly in firmware from stored OS, not by cycling.

**Tap/hold timing:**
- `TAPPING_TERM` = 200 ms
- `ONE_SHOT_TIMEOUT` = 1000 ms

**Mouse key speed** (no Vial panel for these — must reflash to change):
- `MOUSEKEY_MOVE_DELTA` = 8 (pixels per step at minimum speed)
- `MOUSEKEY_MAX_SPEED` = 10 (multiplier ceiling → 80 px/step at max)
- `MOUSEKEY_TIME_TO_MAX` = 30 (steps to reach max speed)
- `MOUSEKEY_INTERVAL` = 16 ms (~60 fps)

Add a block comment explaining ACCEL0/1/2 relationship to speed in plain terms.

---

## rgb_matrix_user.inc

This file contains exactly three lines of content (plus a leading comment block). Each line calls `RGB_MATRIX_EFFECT(name)` with one of: `FIRMWARE_UI`, `DEBUG_MODE`, `GAMING_MODE`. The names must exactly match the function names used in the effect stubs in `keymap.c`. QMK includes this file automatically when `RGB_MATRIX_CUSTOM_USER = yes` — no explicit include directive is needed in any other file.

---

## theme.h

Create at `keymaps/via/theme.h`. Guard with `#pragma once`. Include `quantum.h`.

This file is the **only** source of RGB color values in the entire firmware. `keymap.c` must never contain raw `{r, g, b}` values — every color reference goes through a named constant from this file.

### rgb_t type

Define a typedef struct named `rgb_t` with three `uint8_t` fields: `r`, `g`, `b`.

### COLOR_* constants

Define all constants as `#define NAME ((rgb_t){r, g, b})` macros. Group them into sections with block comments. Values:

**OS indicator colors** — painted on LEDs 0–3 (left top row) to show the stored OS:

| Name | R | G | B | Notes |
|---|---|---|---|---|
| `COLOR_OS_LINUX` | 0 | 80 | 200 | Cyan-blue |
| `COLOR_OS_WINDOWS` | 0 | 120 | 255 | Bright blue |
| `COLOR_OS_MAC` | 180 | 180 | 180 | Neutral gray |
| `COLOR_OS_ANDROID` | 0 | 150 | 0 | Green |

**Modifier colors** — painted on modifier key LEDs. Single mod held = the mod's own color at full brightness. Same mod idle = same color at brightness ÷ `MOD_DIM_FACTOR`. Two specific combos get a blend color shown on both keys:

| Name | R | G | B | Used when |
|---|---|---|---|---|
| `COLOR_MOD_SHIFT` | 255 | 0 | 0 | Shift key, Ctrl not held |
| `COLOR_MOD_CTRL` | 255 | 120 | 0 | Ctrl key, Shift not held |
| `COLOR_MOD_ALT` | 255 | 200 | 0 | Alt key, GUI not held |
| `COLOR_MOD_GUI` | 200 | 0 | 200 | GUI key, Alt not held |
| `COLOR_MOD_SHIFT_CTRL` | 255 | 255 | 0 | Shift LED when Ctrl held, or Ctrl LED when Shift held |
| `COLOR_MOD_ALT_GUI` | 255 | 0 | 255 | Alt LED when GUI held, or GUI LED when Alt held |

**Persist state colors** — defined but not currently used in keymap.c. Retain for future persistent-layer indicators. Do not remove.

| Name | R | G | B |
|---|---|---|---|
| `COLOR_PERSIST_ON` | 0 | 200 | 0 |
| `COLOR_PERSIST_OFF` | 200 | 0 | 0 |

**Layer indicator colors** — painted on LEDs 29–32 (right top row) when a non-BASE layer is active:

| Name | R | G | B | Layer |
|---|---|---|---|---|
| `COLOR_LAYER_FN` | 255 | 100 | 0 | FN (layer 1) |
| `COLOR_LAYER_NAV` | 150 | 0 | 255 | NAV/mouse (layer 2) |
| `COLOR_LAYER_GAME` | 0 | 255 | 0 | GAME (layer 3) |

**Lock state colors** — painted on specific LEDs when OS lock states are active:

| Name | R | G | B | Used for |
|---|---|---|---|---|
| `COLOR_LOCK_CAPS` | 255 | 0 | 0 | Caps Lock → always `LED_IDX_CAPS` |
| `COLOR_LOCK_NUM` | 0 | 200 | 100 | Num Lock → LED whose keycode = KC_NUM |
| `COLOR_LOCK_SCRL` | 200 | 100 | 0 | Scroll Lock → LED whose keycode = KC_SCRL |

**Utility colors:**

| Name | R | G | B | Used for |
|---|---|---|---|---|
| `COLOR_WHITE` | 255 | 255 | 255 | Boot animation lit phase |
| `COLOR_OFF` | 0 | 0 | 0 | Boot animation dark phase + RGB effect stubs canvas clear |

**Theme-slot colors** — 4 colors per theme. These map directly to the 4 fields of `rgb_theme_t` (see below). Define all 12 as named constants:

UI theme (calm blue scheme):

| Name | R | G | B |
|---|---|---|---|
| `COLOR_BASE_UI` | 0 | 40 | 120 |
| `COLOR_ACCENT_UI` | 0 | 0 | 255 |
| `COLOR_BODY_UI` | 255 | 0 | 255 |
| `COLOR_REACTIVE_UI` | 255 | 200 | 0 |

Debug theme (red diagnostic scheme):

| Name | R | G | B |
|---|---|---|---|
| `COLOR_BASE_DEBUG` | 120 | 0 | 0 |
| `COLOR_ACCENT_DEBUG` | 255 | 0 | 0 |
| `COLOR_BODY_DEBUG` | 255 | 100 | 100 |
| `COLOR_REACTIVE_DEBUG` | 255 | 255 | 255 |

Gaming theme (green gaming scheme):

| Name | R | G | B |
|---|---|---|---|
| `COLOR_BASE_GAMING` | 0 | 120 | 0 |
| `COLOR_ACCENT_GAMING` | 0 | 255 | 0 |
| `COLOR_BODY_GAMING` | 255 | 255 | 0 |
| `COLOR_REACTIVE_GAMING` | 255 | 100 | 0 |

### rgb_theme_t struct

Define a typedef struct named `rgb_theme_t` with four `rgb_t` fields:
- `base` — background color for all non-indicator LEDs (painted in render step 1)
- `accent` — color for action keys: Enter, Backspace, Delete, Space (render step 2)
- `body` — color for letters A–Z, digits 1–9 and 0, function keys F1–F12 (render step 3)
- `reactive` — color for keypress fade-out animation in GAMING_MODE only (render step 8)

The `accent` and `body` fields have nothing to do with keyboard layers — they name the visual role of the keys they color.

### Theme instances

Declare three `static const rgb_theme_t` instances using named field initializers (`.base = ...` style):

- `theme_ui`: base=COLOR_BASE_UI, accent=COLOR_ACCENT_UI, body=COLOR_BODY_UI, reactive=COLOR_REACTIVE_UI
- `theme_debug`: base=COLOR_BASE_DEBUG, accent=COLOR_ACCENT_DEBUG, body=COLOR_BODY_DEBUG, reactive=COLOR_REACTIVE_DEBUG
- `theme_gaming`: base=COLOR_BASE_GAMING, accent=COLOR_ACCENT_GAMING, body=COLOR_BODY_GAMING, reactive=COLOR_REACTIVE_GAMING

These are indexed by `ui_mode_t` values (0, 1, 2) in keymap.c via `theme_table[]`.

### apply_color utility

Define a `static inline void apply_color(uint8_t led_index, rgb_t color, uint8_t brightness)` function. It calls `rgb_matrix_set_color` on the given LED index. Each channel value is computed as `(uint8_t)(((uint16_t)channel * brightness) >> 8)`. The shift by 8 is intentional — equivalent to dividing by 256 rather than 255, which is 0.4% off at maximum brightness (imperceptible) but avoids a division instruction on the MCU. This function is the **only** way any code in keymap.c should set LED colors. Never call `rgb_matrix_set_color` directly.

---

## keymap.c

### Includes

Include four headers: `QMK_KEYBOARD_H` (unquoted macro form), `"theme.h"`, `"eeconfig.h"`, `"print.h"`, `"host.h"`.

### Debug log system

Define five numeric levels: DEBUG_NONE=0, DEBUG_ERROR=1, DEBUG_WARN=2, DEBUG_INFO=3, DEBUG_VERBOSE=4.

Guard `DEBUG_LEVEL` with `#ifndef` defaulting to DEBUG_NONE. Define a `LOG(level, fmt, ...)` macro that calls `uprintf` only when `DEBUG_LEVEL >= level`, appending `"\n"` to the format string. Wrap in `do { ... } while(0)`.

### Enums

Three enums:

**layer_id_t** — LAYER_BASE=0, LAYER_FN=1, LAYER_NAV=2, LAYER_GAME=3.

**ui_mode_t** — UI_MODE_DEFAULT=0, UI_MODE_DEBUG=1, UI_MODE_GAMING=2, UI_MODE_COUNT=3. These are the three firmware UI modes that each have a matching RGB effect and theme profile. Declare a static variable `active_ui_mode` of this type, initialized to UI_MODE_DEFAULT.

**os_type_t** — OS_LINUX=0, OS_WINDOWS=1, OS_MAC=2, OS_ANDROID=3, OS_COUNT=4.

### EEPROM config

Define a `user_config_t` union with a `uint32_t raw` field and an anonymous struct containing two bitfields: `os_type` (8 bits) and `theme` (8 bits). Declare a static `user_config_t user_config`.

Define `eeconfig_init_user`: sets os_type=OS_WINDOWS, theme=UI_MODE_DEFAULT, then calls `eeconfig_update_user(user_config.raw)`. This runs when EEPROM is reset/wiped.

Define `load_config`: reads raw from `eeconfig_read_user()`, then bounds-checks both fields — if os_type >= OS_COUNT reset to OS_WINDOWS; if theme >= UI_MODE_COUNT reset to UI_MODE_DEFAULT. Log at DEBUG_INFO level.

Define `save_config`: calls `eeconfig_update_user(user_config.raw)`. Log at DEBUG_INFO level.

### Custom keycodes

Define an enum starting at SAFE_RANGE: OS_SET_LINUX, OS_SET_WINDOWS, OS_SET_MAC, OS_SET_ANDROID, THEME_NEXT, THEME_PREV. In that order.

### Layer 0 — BASE

Full 4-row + 3-thumb LAYOUT. The LAYOUT macro for this keyboard has 58 key positions: 6+6 per row × 4 rows plus 3+3 thumb keys, plus 2 inner-column extra keys per row (rows 1–3) and 1 inner-column key each for rows 1 and 3.

Key assignments row by row (left half then right half, inner-column keys noted separately):

Row 0 (number row): KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5 | KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC.

Row 1 (QWERTY top): KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T with inner-column left = KC_UP | inner-column right = KC_LEFT, then KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC.

Row 2 (home row): KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G with inner-column left = KC_DOWN | inner-column right = KC_RGHT, then KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT. Note: the last key on right is KC_QUOT (apostrophe/single-quote), not Enter and not KC_NO — it is required as the trigger key for the AltGr+' → ä override.

Row 3 (bottom row): KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B with inner-column left = KC_MUTE | inner-column right = RGB_MOD, then KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_DEL.

Thumb cluster: KC_LALT, MO(LAYER_FN), KC_SPC | KC_ENT, MO(LAYER_NAV), KC_RCTL.

Add a comment above this layer listing the three AltGr combinations: AltGr+[ → å, AltGr+; → ö, AltGr+' → ä.

### Layer 1 — FN

Row 0: KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5 | KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_DEL.

Row 1: _______, _______, _______, _______, KC_PGUP, KC_HOME, inner-left=KC_PGUP | inner-right=KC_PGDN, KC_F11, KC_F12, _______, _______, _______, KC_RBRC. The three transparent keys on right (positions after F12 and before RBRC) are intentionally free — add a comment listing potential future assignments: KC_PSCR, QK_LOCK (Caps Word), LSG(KC_S) (Win snip), KC_NUM, KC_SCRL.

Row 2: _______, THEME_PREV, _______, THEME_NEXT, KC_PGDN, KC_END, inner-left=KC_PGDN | inner-right=KC_END, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_QUOT, _______.

Row 3: _______, _______, _______, _______, TG(LAYER_GAME), QK_BOOT, inner-left=KC_VOLD | inner-right=KC_VOLU, OS_SET_WINDOWS, OS_SET_MAC, OS_SET_ANDROID, OS_SET_LINUX, KC_BSLS, _______.

Thumb: _______, MO(LAYER_FN), _______ | _______, MO(LAYER_NAV), _______.

### Layer 2 — NAV / MOUSE

Comment: right HJKL = mouse movement (VIM-style). Left A/S/D = speed modifiers. Inner columns = scroll wheel. Right Y/U/I = mouse buttons.

Row 0: all _______.

Row 1: _______, _______, _______, _______, KC_PGUP, KC_HOME, inner-left=KC_MS_WH_UP | inner-right=KC_MS_WH_DOWN, KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3, _______, _______, _______.

Row 2: _______, KC_MS_ACCEL2, KC_MS_ACCEL1, KC_MS_ACCEL0, KC_PGDN, KC_END, inner-left=KC_MS_WH_DOWN | inner-right=KC_MS_WH_UP, KC_MS_LEFT, KC_MS_DOWN, KC_MS_UP, KC_MS_RGHT, _______, _______.

Row 3: all _______.

Thumb: _______, MO(LAYER_FN), _______ | _______, MO(LAYER_NAV), _______.

Speed notes in comment: ACCEL2=fast, ACCEL1=medium, ACCEL0=sniper (slowest). Hold A/S/D on left hand while moving with HJKL on right hand for two-hand precision control.

### Layer 3 — GAME

All transparent (_______) except: row 2 position 0 (Caps Lock position) = KC_LCTL. This is the only difference from BASE. Comment: Caps → Ctrl is the standard gaming remap. RGB auto-switches to UI_MODE_GAMING when this layer activates.

Thumb: _______, MO(LAYER_FN), _______ | _______, MO(LAYER_NAV), _______.

### Encoder map

Four entries (one per layer), two encoders each, using ENCODER_CCW_CW(ccw, cw) macro. Wrap in `#if defined(ENCODER_MAP_ENABLE)`:

- Layer 0: encoder 0 = CCW→KC_RIGHT / CW→KC_LEFT; encoder 1 = CCW→KC_VOLD / CW→KC_VOLU
- Layer 1: encoder 0 = CCW→RGB_HUD / CW→RGB_HUI; encoder 1 = CCW→RGB_SAD / CW→RGB_SAI
- Layer 2: encoder 0 = CCW→RGB_VAD / CW→RGB_VAI; encoder 1 = CCW→RGB_SPD / CW→RGB_SPI
- Layer 3: encoder 0 = CCW→RGB_RMOD / CW→RGB_MOD; encoder 1 = CCW→KC_RIGHT / CW→KC_LEFT

Note: encoder 0 BASE layer is CCW→RIGHT / CW→LEFT — this is intentionally reversed from intuition. It matches the vendor firmware; do not change.

### Combos

Declare `const uint16_t PROGMEM combo_placeholder[]` containing only `COMBO_END`. Declare `combo_t key_combos[COMBO_COUNT]` as an empty array (`= {}`). COMBO_COUNT is 0. This satisfies the combo system's requirements while having no hardcoded combos — all combos are managed at runtime via Vial's combo editor.

### Key overrides — Swedish AltGr characters

Declare three `static const key_override_t` variables using `ko_make_basic(mod_mask, trigger_key, replacement_keycode)`:

- `ao_override`: mod = MOD_BIT(KC_RALT), trigger = KC_LBRC, replacement = UC(0x00E5) → produces å
- `oo_override`: mod = MOD_BIT(KC_RALT), trigger = KC_SCLN, replacement = UC(0x00F6) → produces ö
- `ae_override`: mod = MOD_BIT(KC_RALT), trigger = KC_QUOT, replacement = UC(0x00E4) → produces ä

Declare `const key_override_t *key_overrides[]` array containing pointers to all three in that order, terminated with NULL.

Important: KC_QUOT must be an actual key in the BASE layer (not KC_NO) because ko_make_basic requires the trigger key to produce a real keycode. The BASE layer R76 position is KC_QUOT for this reason.

### LED zone definitions

Declare `static const uint8_t os_led_zone[]` containing LEDs 0, 1, 2, 3 (left top row keys). Define `OS_LED_ZONE_SIZE` as its sizeof-based count.

Declare `static const uint8_t layer_led_zone[]` containing LEDs 29, 30, 31, 32 (right top row keys). Define `LAYER_LED_ZONE_SIZE` as its sizeof-based count.

Define `LED_IDX_CAPS` = 13. This is matrix row 2, col 0 — the Caps Lock position. Hardcoded so Caps Lock indicator always works on every layer without cache lookups.

Declare two helper functions: `led_in_os_zone(uint8_t led)` iterates `os_led_zone[]` and returns true if the LED is in it. `led_in_layer_zone(uint8_t led)` iterates `layer_led_zone[]` and returns true if the LED is in it. Both return false by default.

### Highlight keys list

Declare `static const uint16_t highlight_keys[]` containing KC_ENT, KC_BSPC, KC_DEL, KC_SPC. Define `HIGHLIGHT_KEY_COUNT` as its sizeof-based count. These keys get the `theme->accent` color in render step 2.

### Keycode cache

Conditionally define `M57_LED_COUNT` as `RGB_MATRIX_LED_COUNT` (if defined) or 58 as fallback.

Declare `static uint16_t led_key_cache[M57_LED_COUNT]` — stores the effective keycode for each LED index on the currently active layer.

Declare `static uint32_t cache_refresh_timer = 0`. Define `CACHE_REFRESH_MS` = 100. Define `MOD_DIM_FACTOR` = 4.

Declare `static uint8_t mod_leds[MOD_LED_MAX]` and `static uint8_t mod_led_count = 0`. Define `MOD_LED_MAX` = 16.

Define `is_mod_key(uint16_t kc)` as a static inline returning true when kc is in the range KC_LCTL through KC_RGUI (inclusive).

Define `refresh_key_cache()`: iterates all MATRIX_ROWS × MATRIX_COLS positions. For each position, looks up the LED index from `g_led_config.matrix_co[row][col]`. If not NO_LED, writes `keymap_key_to_keycode(layer, {row, col})` to `led_key_cache[led]`. The layer is `get_highest_layer(layer_state)`. **Critical: always use `keymap_key_to_keycode()`, never read `keymaps[][]` directly — Vial remaps only show up through this function.**

Define `refresh_mod_cache()`: resets `mod_led_count` to 0, iterates all LEDs up to M57_LED_COUNT, appends LED index to `mod_leds[]` when `is_mod_key(led_key_cache[i])` is true, stopping when `mod_led_count` reaches MOD_LED_MAX.

### Reactive system

Used for keypress fade-out animation in UI_MODE_GAMING. Define:

- `REACTIVE_BUF_SIZE` = 8 — ring buffer capacity
- `REACTIVE_FADE_MS` = 300 — fade-out duration per keypress event
- `REACTIVE_SLOT_FREE` = 0xFF — sentinel value marking an empty slot

Define a struct `reactive_hit_t` with two fields: `uint8_t led` (set to REACTIVE_SLOT_FREE when empty) and `uint32_t timestamp`.

Declare `static reactive_hit_t reactive_hits[REACTIVE_BUF_SIZE]` and `static uint8_t reactive_write_pos = 0` (ring buffer write pointer).

### Boot animation constants

Define: `BOOT_SWEEP_PHASE_MS` = 250 (four phases × 250 ms = 1000 ms total), `BOOT_LED_MAX_X` = 210 (maximum x-coordinate in `g_led_config.point[]`), `BOOT_ANIM_BRIGHTNESS` = 255.

Declare `static bool boot_anim_active = false` and `static uint32_t boot_anim_timer = 0`.

### Theme selection

Declare `static const rgb_theme_t* const theme_table[UI_MODE_COUNT]` array indexed by `ui_mode_t`: index 0 → &theme_ui, index 1 → &theme_debug, index 2 → &theme_gaming.

Define `get_active_theme()` returning a `const rgb_theme_t*`. It returns `theme_table[user_config.theme]` if in bounds, otherwise logs a warning and returns `&theme_ui`.

### Render pipeline — 8 steps

All steps run in `rgb_matrix_indicators_advanced_user`. The pipeline only runs in custom RGB modes (when `rgb_matrix_get_mode() >= RGB_MATRIX_CUSTOM`). Each step receives at minimum `led_min`, `led_max`, and `brightness` (from `rgb_matrix_get_val()`). Steps that color by theme also receive `const rgb_theme_t* theme` from `get_active_theme()`.

**Step 1 — render_background**: fills every LED from led_min to led_max with `theme->base` at `brightness`. Skips any LED where `led_in_os_zone()` returns true.

**Step 2 — render_action_keys**: iterates LEDs led_min to led_max, skips OS zone LEDs. For each LED, checks its cached keycode against every entry in `highlight_keys[]`. If it matches, paints that LED with `theme->accent` at `brightness`.

**Step 3 — render_key_classes**: iterates LEDs, skips OS zone LEDs. Paints `theme->body` at `brightness` on any LED whose cached keycode is: any letter (KC_A through KC_Z), any digit (KC_1 through KC_9, or KC_0), or any function key (KC_F1 through KC_F12).

**Step 4 — render_mod_keys**: iterates only `mod_leds[]` (the precomputed subset). Skips LEDs outside led_min..led_max range. Skips OS zone LEDs. For each modifier LED, determines color and active state from the cached keycode plus current mods (`get_mods() | get_oneshot_mods()`):
- KC_LSFT or KC_RSFT: color = COLOR_MOD_SHIFT_CTRL if Ctrl is held, otherwise COLOR_MOD_SHIFT; active = Shift mask nonzero
- KC_LCTL or KC_RCTL: color = COLOR_MOD_SHIFT_CTRL if Shift is held, otherwise COLOR_MOD_CTRL; active = Ctrl mask nonzero
- KC_LALT or KC_RALT: color = COLOR_MOD_ALT_GUI if GUI is held, otherwise COLOR_MOD_ALT; active = Alt mask nonzero
- KC_LGUI or KC_RGUI: color = COLOR_MOD_ALT_GUI if Alt is held, otherwise COLOR_MOD_GUI; active = GUI mask nonzero
- Brightness when active: full `brightness`. Brightness when idle: `brightness / MOD_DIM_FACTOR`.

**Step 5 — render_os_indicator**: iterates `os_led_zone[]`. For each LED in range, paints it with the OS color (switch on `user_config.os_type` using the COLOR_OS_* constants; default = COLOR_OFF). This step always paints the OS zone regardless of what previous steps did.

**Step 6 — render_layer_indicator**: gets `get_highest_layer(layer_state)`. On LAYER_BASE: returns immediately (no paint — theme base color from step 1 is correct). On other layers: iterates `layer_led_zone[]`, paints LEDs in range with the layer's color (COLOR_LAYER_FN, COLOR_LAYER_NAV, COLOR_LAYER_GAME).

**Step 7 — render_lock_indicators**: reads `host_keyboard_led_state()`. If caps_lock is active and `LED_IDX_CAPS` is in range: paint that LED with COLOR_LOCK_CAPS. If num_lock or scroll_lock is active: iterate all LEDs; if num_lock and keycode = KC_NUM → paint COLOR_LOCK_NUM; if scroll_lock and keycode = KC_SCRL → paint COLOR_LOCK_SCRL.

**Step 8 — render_reactive** (UI_MODE_GAMING only): iterates `reactive_hits[]`. For each slot where `led != REACTIVE_SLOT_FREE`: compute elapsed = `timer_elapsed32(timestamp)`. If elapsed >= REACTIVE_FADE_MS: mark slot as REACTIVE_SLOT_FREE and continue. Otherwise: skip if LED out of range, skip if `led_in_os_zone()` or `led_in_layer_zone()`. Compute intensity = `(uint8_t)(((uint32_t)(REACTIVE_FADE_MS - elapsed) * brightness) / REACTIVE_FADE_MS)`. Paint with `apply_color(led, theme->reactive, intensity)`.

### Custom RGB effect stubs

Wrap in `#ifdef RGB_MATRIX_CUSTOM_USER`. Define three boolean effect functions: `FIRMWARE_UI`, `DEBUG_MODE`, `GAMING_MODE` — each taking `effect_params_t* params`. Each uses the `RGB_MATRIX_USE_LIMITS(led_min, led_max)` macro to get the LED range, iterates all LEDs in range painting them COLOR_OFF at full brightness via `apply_color`, then returns `led_max < RGB_MATRIX_LED_COUNT`. These stubs just clear the canvas — the actual rendering happens in `rgb_matrix_indicators_advanced_user`.

### encoder_update_user

Reads `get_highest_layer(layer_state)` and `rgb_matrix_get_mode()`. Determines `gaming_rgb_active` as true when the mode is >= RGB_MATRIX_CUSTOM and the mode index minus RGB_MATRIX_CUSTOM equals UI_MODE_GAMING.

If layer is LAYER_FN **and** `gaming_rgb_active` is false:
- Encoder 0 (left): clockwise increments `user_config.theme` modulo UI_MODE_COUNT; counter-clockwise decrements (with wraparound). Log at DEBUG_INFO.
- Encoder 1 (right): clockwise increments `user_config.os_type` modulo OS_COUNT; counter-clockwise decrements (with wraparound). Then immediately call `unicode_input_mode_set()` with the matching unicode mode for the new OS (see unicode sync section below). Log at DEBUG_INFO.

After either encoder action: call `save_config()`. Return false (consumed).

If the condition is not met: return true (let encoder_map handle it).

### process_record_user

Only act on key press events (`record->event.pressed` is true). Return true for unhandled keycodes.

For OS_SET_LINUX: set `user_config.os_type = OS_LINUX`, call `save_config()`, call `unicode_input_mode_set(UNICODE_MODE_LINUX)`. Return false.

For OS_SET_WINDOWS: set OS_WINDOWS, save, `unicode_input_mode_set(UNICODE_MODE_WIN)`. Return false.

For OS_SET_MAC: set OS_MAC, save, `unicode_input_mode_set(UNICODE_MODE_MACOS)`. Return false.

For OS_SET_ANDROID: set OS_ANDROID, save, `unicode_input_mode_set(UNICODE_MODE_LINUX)` (Android uses Linux method). Return false.

For THEME_NEXT: increment `user_config.theme` modulo UI_MODE_COUNT, save. Return false.

For THEME_PREV: decrement with wraparound, save. Return false.

After the switch statement: check if UI_MODE_GAMING is currently active (same `gaming_rgb_active` check as encoder_update_user). If so, look up the LED for the pressed key via `g_led_config.matrix_co[row][col]`. If not NO_LED: write that LED index and `timer_read32()` into `reactive_hits[reactive_write_pos]`, then advance `reactive_write_pos = (reactive_write_pos + 1) % REACTIVE_BUF_SIZE`.

Return true at the end.

### keyboard_post_init_user

Call `load_config()`.

Call `unicode_input_mode_set()` based on stored OS: OS_MAC → UNICODE_MODE_MACOS; OS_LINUX or OS_ANDROID → UNICODE_MODE_LINUX; anything else (including OS_WINDOWS) → UNICODE_MODE_WIN.

Initialize reactive hits buffer: set all `reactive_hits[i].led = REACTIVE_SLOT_FREE` for i = 0 to REACTIVE_BUF_SIZE-1.

Call `refresh_key_cache()` and `refresh_mod_cache()`.

Set `boot_anim_active = true` and `boot_anim_timer = timer_read32()`.

Log at DEBUG_INFO: LED count, modifier count, os, theme.

### layer_state_set_user

Call `refresh_key_cache()` and `refresh_mod_cache()`.

Manage GAME-layer RGB auto-switch with two static variables: `game_layer_was_on` (bool, false) and `saved_rgb_mode` (uint8_t, 0).

Check `is_gaming` = state has LAYER_GAME bit set. If is_gaming and not `game_layer_was_on`: save current mode to `saved_rgb_mode`, switch to `RGB_MATRIX_CUSTOM + UI_MODE_GAMING`. If not is_gaming and `game_layer_was_on`: restore `saved_rgb_mode`. Update `game_layer_was_on`. Log.

Return state unchanged.

### rgb_matrix_indicators_advanced_user

**Boot animation block** (runs first, overrides everything else): if `boot_anim_active`, compute `elapsed = timer_elapsed32(boot_anim_timer)`. If elapsed >= BOOT_SWEEP_PHASE_MS × 4: set `boot_anim_active = false` and fall through. Otherwise: compute `phase = elapsed / BOOT_SWEEP_PHASE_MS` (integer, 0–3) and `phase_t = elapsed - phase * BOOT_SWEEP_PHASE_MS`. Compute sweep edge: `sweep = (phase_t * BOOT_LED_MAX_X) / BOOT_SWEEP_PHASE_MS`. For each LED i from led_min to led_max: read `lx = g_led_config.point[i].x`. Compute `lit` from phase: phase 0 → lit = (lx <= sweep); phase 1 → lit = (lx > sweep); phase 2 → lit = (lx >= BOOT_LED_MAX_X - sweep); phase 3 → lit = (lx < BOOT_LED_MAX_X - sweep). Paint `apply_color(i, lit ? COLOR_WHITE : COLOR_OFF, BOOT_ANIM_BRIGHTNESS)`. Return false (skip rest of pipeline).

**Custom mode gate**: read mode = `rgb_matrix_get_mode()`. If mode < RGB_MATRIX_CUSTOM: return false (let QMK handle standard modes).

Otherwise: set `active_ui_mode = (ui_mode_t)(mode - RGB_MATRIX_CUSTOM)`.

**Periodic cache refresh**: if `timer_elapsed32(cache_refresh_timer) > CACHE_REFRESH_MS`: call `refresh_key_cache()`, `refresh_mod_cache()`, reset `cache_refresh_timer = timer_read32()`. This catches Vial remaps applied at runtime without a layer change.

Get `theme = get_active_theme()` and `brightness = rgb_matrix_get_val()`.

Run render steps 1–7 in order (render_background, render_action_keys, render_key_classes, render_mod_keys, render_os_indicator, render_layer_indicator, render_lock_indicators).

If `active_ui_mode == UI_MODE_GAMING`: run step 8 (render_reactive).

Return false.

---

## Unicode sync — critical rule

The unicode input mode must be set in exactly three places. Missing any one of them will cause Swedish characters (å ö ä) to stop working after the OS is changed via that path:

1. `keyboard_post_init_user` — sets mode from stored EEPROM OS on every boot
2. Each OS_SET_* case in `process_record_user` — sets mode immediately on key press
3. Right-encoder OS cycling in `encoder_update_user` — sets mode immediately after cycling

The mapping OS → unicode mode is: OS_WINDOWS → UNICODE_MODE_WIN, OS_MAC → UNICODE_MODE_MACOS, OS_LINUX → UNICODE_MODE_LINUX, OS_ANDROID → UNICODE_MODE_LINUX.

---

## Key design decisions

**`apply_color` uses `>> 8` not `/ 255`**: shift is one instruction on ARM Cortex-M4; division is much more expensive. The 0.4% error at max brightness is imperceptible.

**`led_key_cache` uses `keymap_key_to_keycode()`, never `keymaps[][]`**: Vial remaps are stored in EEPROM and returned by this function; direct array access returns the compile-time value and ignores user remaps.

**`reactive_hits` buffer filled only in UI_MODE_GAMING**: avoids wasting buffer slots when they'll never be rendered.

**OS zone excluded from all render steps except render_os_indicator**: prevents any theme color from leaking into the OS zone. Every render step 1–4 checks `led_in_os_zone()` and skips those LEDs.

**Layer zone excluded from render_reactive**: prevents reactive flashes from painting over the layer indicator.

**COMBO_COUNT 0 with empty array**: satisfies the combo subsystem while leaving all runtime combo slots available via Vial.

**GAME layer only changes Caps → Ctrl**: minimal diff; all other keys inherit from BASE via transparent. Keeps the gaming layout simple and maintainable.

**Encoder FN+UI_MODE_GAMING passes through to encoder_map**: prevents accidental OS/theme changes during active gameplay when FN is held.

**Boot animation total duration = 1000 ms**: 4 phases × 250 ms. The sweep is a column-by-column curtain using the x-coordinate from `g_led_config.point[i].x` which ranges 0–210.
