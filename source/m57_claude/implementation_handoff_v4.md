# borne m57 — keymap specification (v4)

Target audience: a QMK developer. No implementation logic is given. Feature and setting names follow standard QMK / firmware convention throughout.

---

## Hardware facts

| Property | Value |
|---|---|
| MCU | STM32F401 |
| Matrix | 10 × 7 unified (rows 0–4 = left, rows 5–9 = right) |
| RGB | 58 WS2812 LEDs, 29 per side, PWM on A7 via TIM3 CH2 |
| Encoders | Left: B8/B9 index 0, Right: B4/B6 index 1, resolution 2 |
| Split transport | Full-duplex USART1, A9/A10, 19200 baud |
| Master side | Right (MASTER_RIGHT) |
| USB VID/PID | 0x6401 / 0x45D4 — goes in `info.json` (`usb.vid` / `usb.pid`) and `vial.json` (`vendorId` / `productId`) |
| Vial UID | 0x45, 0x89, 0xD8, 0xFA, 0xC7, 0x2A, 0x36, 0x89  (compact: 4589D8FAC72A3689) — goes in `keymaps/via/config.h` as `VIAL_KEYBOARD_UID` and in `keymaps/via/vial.json` (see below) |
| EEPROM | Wear-leveling legacy, 4 KB logical / 8 KB backing |

Encoder push positions (matrix L36, R80) have no LED — they map to NO_LED.

---

## rules.mk — feature flags

| Flag | Value | Purpose |
|---|---|---|
| VIAL_ENABLE | yes | Vial protocol |
| VIA_ENABLE | yes | VIA protocol (Vial dependency) |
| VIALRGB_ENABLE | yes | Vial RGB mode/brightness controls |
| RGB_MATRIX_CUSTOM_USER | yes | Includes `rgb_matrix_user.inc` and compiles custom effect stubs |
| ENCODER_MAP_ENABLE | yes | Per-layer encoder action tables |
| COMBO_ENABLE | yes | Required for Vial runtime combo editor even with zero hardcoded combos |
| TAP_DANCE_ENABLE | yes | Tap-dance; slots configured live in Vial |
| KEY_OVERRIDE_ENABLE | yes | Key overrides; used for AltGr Swedish characters; editable in Vial |
| UNICODE_ENABLE | yes | UC() keycodes for Swedish character output |
| AUTO_SHIFT_ENABLE | yes | Hold-for-shift; threshold tunable in Vial |
| MOUSEKEY_ENABLE | yes | Mouse movement/click/scroll keycodes for NAV layer |
| REPEAT_KEY_ENABLE | yes | QK_REPEAT_KEY / QK_ALT_REPEAT_KEY |
| DYNAMIC_TAPPING_TERM_ENABLE | yes | Live tapping term adjustment via DT_UP / DT_DOWN |
| VIAL_INSECURE | yes | Vial unlock without physical key combo |
| CONSOLE_ENABLE | (commented out) | Debug builds only |

---

## config.h — compile-time settings

| Define | Value | Notes |
|---|---|---|
| VIAL_KEYBOARD_UID | 0x45,0x89,0xD8,0xFA,0xC7,0x2A,0x36,0x89 | compact: 4589D8FAC72A3689 |
| COMBO_COUNT | 0 | All combos managed at runtime via Vial |
| VIAL_TAP_DANCE_ENTRIES | 8 | |
| VIAL_COMBO_ENTRIES | 8 | |
| VIAL_KEY_OVERRIDE_ENTRIES | 8 | |
| UNICODE_SELECTED_MODES | UNICODE_MODE_WIN, UNICODE_MODE_MACOS, UNICODE_MODE_LINUX | Populates the UC cycle list; active mode is set explicitly from stored OS |
| TAPPING_TERM | 200 | ms |
| ONE_SHOT_TIMEOUT | 1000 | ms |
| MOUSEKEY_MOVE_DELTA | 8 | px per step at minimum speed |
| MOUSEKEY_MAX_SPEED | 10 | multiplier ceiling |
| MOUSEKEY_TIME_TO_MAX | 30 | steps to reach max speed |
| MOUSEKEY_INTERVAL | 16 | ms (~60 fps) |

---

## info.json — keyboard-level USB identifiers

This file exists at the keyboard level (not inside `keymaps/`). Set the `usb` block:

| Field | Value |
|---|---|
| `usb.vid` | `"0x6401"` |
| `usb.pid` | `"0x45D4"` |
| `usb.device_version` | `"0.0.1"` |

---

## vial.json — Vial protocol identifiers

This file lives in `keymaps/via/vial.json`. It must contain the matching VID/PID so Vial recognizes the keyboard, plus the matrix size and lighting type:

| Field | Value |
|---|---|
| `name` | `"BORNE"` |
| `vendorId` | `"0x6401"` |
| `productId` | `"0x45D4"` |
| `matrix.rows` | `10` |
| `matrix.cols` | `7` |
| `lighting` | `"vialrgb"` |

The `layouts.keymap` array is the KLE layout definition — carry it over from the existing keyboard definition unchanged.

**All three ID locations must agree:**

| Value | File | Field |
|---|---|---|
| VID `0x6401` | `info.json` | `usb.vid` |
| VID `0x6401` | `keymaps/via/vial.json` | `vendorId` |
| PID `0x45D4` | `info.json` | `usb.pid` |
| PID `0x45D4` | `keymaps/via/vial.json` | `productId` |
| UID `4589D8FAC72A3689` | `keymaps/via/config.h` | `VIAL_KEYBOARD_UID` byte array |

---

## EEPROM user config (`eeconfig`)

Store two fields packed into a `uint32_t` via a union (QMK `user_config_t` pattern):

| Field | Bits | Type | Default |
|---|---|---|---|
| os_type | 8 | os_type_t | OS_WINDOWS |
| theme | 8 | ui_mode_t | UI_MODE_DEFAULT |

`eeconfig_init_user` resets to defaults. On load, bounds-check both fields and reset to defaults if out of range.

---

## Custom keycodes (SAFE_RANGE)

| Keycode | Action |
|---|---|
| OS_SET_LINUX | Store OS_LINUX; set UNICODE_MODE_LINUX |
| OS_SET_WINDOWS | Store OS_WINDOWS; set UNICODE_MODE_WIN |
| OS_SET_MAC | Store OS_MAC; set UNICODE_MODE_MACOS |
| OS_SET_ANDROID | Store OS_ANDROID; set UNICODE_MODE_LINUX |
| THEME_NEXT | Increment stored theme (mod UI_MODE_COUNT) |
| THEME_PREV | Decrement stored theme (with wrap) |

---

## Layers

Four layers. Use `layer_id_t` enum: LAYER_BASE=0, LAYER_FN=1, LAYER_NAV=2, LAYER_GAME=3.

### LAYER_BASE

```
KC_ESC   KC_1   KC_2   KC_3   KC_4   KC_5          KC_6   KC_7   KC_8   KC_9   KC_0    KC_BSPC
KC_TAB   KC_Q   KC_W   KC_E   KC_R   KC_T  KC_UP   KC_LEFT KC_Y  KC_U   KC_I   KC_O    KC_P    KC_LBRC
KC_CAPS  KC_A   KC_S   KC_D   KC_F   KC_G  KC_DOWN KC_RGHT KC_H  KC_J   KC_K   KC_L    KC_SCLN KC_QUOT
KC_LSFT  KC_Z   KC_X   KC_C   KC_V   KC_B  KC_MUTE RGB_MOD KC_N  KC_M   KC_COMM KC_DOT KC_SLSH KC_DEL

              KC_LALT  MO(LAYER_FN)  KC_SPC    KC_ENT  MO(LAYER_NAV)  KC_RCTL
```

Note: `KC_QUOT` on row 2 right-end is required — it is the trigger key for the `ä` key override. Must not be KC_NO.

AltGr combinations active on this layer (via key_override):
- AltGr + `[` → å  
- AltGr + `;` → ö  
- AltGr + `'` → ä  

### LAYER_FN

```
KC_GRV   KC_F1  KC_F2  KC_F3  KC_F4  KC_F5         KC_F6  KC_F7  KC_F8  KC_F9  KC_F10  KC_DEL
_______  _____  _____  _____  KC_PGUP KC_HOME KC_PGUP KC_PGDN KC_F11 KC_F12 _____  _____   _____   KC_RBRC
_______  THEME_PREV ___  THEME_NEXT KC_PGDN KC_END KC_PGDN KC_END KC_LEFT KC_DOWN KC_UP KC_RGHT KC_QUOT _______
_______  _____  _____  _____  TG(LAYER_GAME) QK_BOOT KC_VOLD KC_VOLU OS_SET_WINDOWS OS_SET_MAC OS_SET_ANDROID OS_SET_LINUX KC_BSLS _______

              _______  MO(LAYER_FN)  _______    _______  MO(LAYER_NAV)  _______
```

Three right-side transparent keys on row 1 are intentionally free (future: KC_PSCR, QK_LOCK, LSG(KC_S), KC_NUM, KC_SCRL).

### LAYER_NAV (mouse / navigation)

Right HJKL = mouse movement (VIM-style). Left ASD = MOUSEKEY_ACCEL (2=fast, 1=medium, 0=sniper). Inner columns = scroll wheel. Right YUI = mouse buttons.

```
_______  _____  _____  _____  _____  _____                  _____  _____  _____  _____  _____  _______
_______  _____  _____  _____  KC_PGUP KC_HOME KC_MS_WH_UP  KC_MS_WH_DOWN KC_MS_BTN1 KC_MS_BTN2 KC_MS_BTN3 _____ _____ _______
_______  KC_MS_ACCEL2 KC_MS_ACCEL1 KC_MS_ACCEL0 KC_PGDN KC_END KC_MS_WH_DOWN KC_MS_WH_UP KC_MS_LEFT KC_MS_DOWN KC_MS_UP KC_MS_RGHT _____ _______
_______  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _______

              _______  MO(LAYER_FN)  _______    _______  MO(LAYER_NAV)  _______
```

### LAYER_GAME

Identical to LAYER_BASE (all transparent) with one change: row 2 position 0 (KC_CAPS position) → KC_LCTL.

This is the standard gaming remap (Caps → Ctrl). RGB auto-switches to GAMING_MODE effect when this layer activates (see RGB section).

---

## Encoder map (ENCODER_MAP_ENABLE)

| Layer | Encoder 0 (left) CCW/CW | Encoder 1 (right) CCW/CW |
|---|---|---|
| BASE | KC_RIGHT / KC_LEFT | KC_VOLD / KC_VOLU |
| FN | RGB_HUD / RGB_HUI | RGB_SAD / RGB_SAI |
| NAV | RGB_VAD / RGB_VAI | RGB_SPD / RGB_SPI |
| GAME | RGB_RMOD / RGB_MOD | KC_RIGHT / KC_LEFT |

Encoder 0 BASE: CCW=RIGHT, CW=LEFT — intentionally reversed (matches vendor firmware, do not change).

### Encoder overrides in process_record / encoder_update

When **encoder 0 is turned on LAYER_FN** and the active RGB mode is **not GAMING_MODE**:
- CW → THEME_NEXT (cycles `user_config.theme` forward)
- CCW → THEME_PREV

When **encoder 1 is turned on LAYER_FN** and the active RGB mode is **not GAMING_MODE**:
- CW → next OS (cycles `user_config.os_type`), immediately applies matching unicode_input_mode
- CCW → previous OS, same

When GAMING_MODE is active and FN is held, both encoder overrides pass through to the encoder_map (no theme/OS cycling during gameplay).

After either override: `eeconfig_update_user`.

---

## Combos

`COMBO_COUNT = 0`. Declare the required `combo_t key_combos[]` empty array. All combos managed via Vial runtime combo editor.

---

## Key overrides — Swedish AltGr characters

Three `ko_make_basic` overrides, all triggered by `MOD_BIT(KC_RALT)`:

| Trigger | Replacement | Character |
|---|---|---|
| KC_LBRC | UC(0x00E5) | å |
| KC_SCLN | UC(0x00F6) | ö |
| KC_QUOT | UC(0x00E4) | ä |

These are also exposed in Vial's key override editor (VIAL_KEY_OVERRIDE_ENTRIES = 8).

---

## Unicode input mode sync

The active unicode input mode must always match the stored `os_type`. Set it in three places:
1. `keyboard_post_init_user` — from EEPROM on boot
2. Each OS_SET_* keycode handler in `process_record_user`
3. Encoder 1 OS cycling in `encoder_update_user`

Mapping: OS_WINDOWS → UNICODE_MODE_WIN, OS_MAC → UNICODE_MODE_MACOS, OS_LINUX / OS_ANDROID → UNICODE_MODE_LINUX.

---

## RGB system

### rgb_matrix_user.inc

Declares three custom effects: `FIRMWARE_UI`, `DEBUG_MODE`, `GAMING_MODE`. These correspond 1:1 with the three `ui_mode_t` values (0, 1, 2).

### theme.h (create from scratch)

Defines:
- `rgb_t` — struct with uint8_t r, g, b
- `apply_color(led_index, rgb_t, brightness)` — the only function that calls `rgb_matrix_set_color`; applies brightness scaling per channel
- All `COLOR_*` constants as `(rgb_t){r,g,b}` macros
- `rgb_theme_t` — struct with four `rgb_t` fields: base, accent, body, reactive
- Three `const rgb_theme_t` instances: `theme_ui`, `theme_debug`, `theme_gaming`

### LED zones

| Zone | Name | LED indices | Purpose |
|---|---|---|---|
| OS indicator | `os_led_zone[]` | 0, 1, 2, 3 | Left top row — shows stored OS |
| Layer indicator | `layer_led_zone[]` | 29, 30, 31, 32 | Right top row — shows active layer |
| Caps Lock | `LED_IDX_CAPS` = 13 | single LED | Caps Lock indicator; hardcoded |

### Color palette

**OS indicator colors** (painted on os_led_zone):

| Constant | R | G | B |
|---|---|---|---|
| COLOR_OS_LINUX | 0 | 80 | 200 |
| COLOR_OS_WINDOWS | 0 | 120 | 255 |
| COLOR_OS_MAC | 180 | 180 | 180 |
| COLOR_OS_ANDROID | 0 | 150 | 0 |

**Layer indicator colors** (painted on layer_led_zone):

| Constant | R | G | B | Layer |
|---|---|---|---|---|
| COLOR_LAYER_FN | 255 | 100 | 0 | LAYER_FN |
| COLOR_LAYER_NAV | 150 | 0 | 255 | LAYER_NAV |
| COLOR_LAYER_GAME | 0 | 255 | 0 | LAYER_GAME |

**Modifier colors** (paired dim/bright per-key):

| Constant | R | G | B | When used |
|---|---|---|---|---|
| COLOR_MOD_SHIFT | 255 | 0 | 0 | Shift, Ctrl not held |
| COLOR_MOD_CTRL | 255 | 120 | 0 | Ctrl, Shift not held |
| COLOR_MOD_ALT | 255 | 200 | 0 | Alt, GUI not held |
| COLOR_MOD_GUI | 200 | 0 | 200 | GUI, Alt not held |
| COLOR_MOD_SHIFT_CTRL | 255 | 255 | 0 | Shift↔Ctrl combo blend |
| COLOR_MOD_ALT_GUI | 255 | 0 | 255 | Alt↔GUI combo blend |

Idle modifier brightness = active brightness ÷ `MOD_DIM_FACTOR` (= 4).

**Lock indicator colors:**

| Constant | R | G | B | Lock |
|---|---|---|---|---|
| COLOR_LOCK_CAPS | 255 | 0 | 0 | Caps Lock → LED_IDX_CAPS |
| COLOR_LOCK_NUM | 0 | 200 | 100 | Num Lock → LED matching KC_NUM in cache |
| COLOR_LOCK_SCRL | 200 | 100 | 0 | Scroll Lock → LED matching KC_SCRL in cache |

**Utility:**

| Constant | R | G | B | Used for |
|---|---|---|---|---|
| COLOR_WHITE | 255 | 255 | 255 | Boot animation lit phase |
| COLOR_OFF | 0 | 0 | 0 | Boot animation dark phase; effect canvas clear |
| COLOR_PERSIST_ON | 0 | 200 | 0 | Reserved (not used) |
| COLOR_PERSIST_OFF | 200 | 0 | 0 | Reserved (not used) |

**Per-theme colors** (maps to rgb_theme_t fields):

| Theme | base | accent | body | reactive |
|---|---|---|---|---|
| theme_ui (UI_MODE_DEFAULT) | 0,40,120 | 0,0,255 | 255,0,255 | 255,200,0 |
| theme_debug (UI_MODE_DEBUG) | 120,0,0 | 255,0,0 | 255,100,100 | 255,255,255 |
| theme_gaming (UI_MODE_GAMING) | 0,120,0 | 0,255,0 | 255,255,0 | 255,100,0 |

`rgb_theme_t` field semantics:
- `base` — background color for all non-indicator LEDs
- `accent` — action keys: Enter, Backspace, Delete, Space
- `body` — letters A–Z, digits 0–9, F1–F12
- `reactive` — keypress fade-out in GAMING_MODE

### rgb_matrix_indicators_advanced_user — visual behavior

Implement as `rgb_matrix_indicators_advanced_user`. Active only when `rgb_matrix_get_mode() >= RGB_MATRIX_CUSTOM`. The active ui_mode is derived from the current RGB mode index.

**Boot animation** (runs on startup, overrides everything else for ~1 second):
Left-to-right sweep using each LED's x-coordinate from `g_led_config.point[i].x` (range 0–210). Four phases, 250 ms each: sweep white in, sweep dark in from right, sweep white out from right, sweep dark out. After 1000 ms, animation ends and normal rendering takes over.

**Per-frame render order** (each step paints over the previous where they overlap; order is fixed):
1. **Background** — fill all non-os-zone LEDs with `theme->base`
2. **Accent keys** — paint Enter/Backspace/Delete/Space with `theme->accent`
3. **Body keys** — paint letters, digits, F-keys with `theme->body`
4. **Modifier keys** — paint KC_LCTL/RCTL/LSFT/RSFT/LALT/RALT/LGUI/RGUI with their mod color, dim when idle
5. **OS indicator** — paint os_led_zone with COLOR_OS_* for stored os_type (always overwrites)
6. **Layer indicator** — paint layer_led_zone with COLOR_LAYER_* for highest active layer (skip on LAYER_BASE)
7. **Lock indicators** — paint Caps/Num/Scroll lock LEDs when their OS lock state is active
8. **Reactive** (GAMING_MODE only) — fade-out animation on recently pressed key LEDs (skip os/layer zones)

Steps 1–4 all skip os_led_zone LEDs. Step 8 skips both os_led_zone and layer_led_zone.

### Keycode-to-LED cache

Maintain a per-LED keycode lookup table (`led_key_cache[LED_COUNT]`) built from `keymap_key_to_keycode()` on the current highest layer. Refresh on layer change (`layer_state_set_user`) and periodically every ~100 ms in `rgb_matrix_indicators_advanced_user` to catch live Vial remaps. **Always use `keymap_key_to_keycode()`, never access `keymaps[][]` directly** — Vial remaps are only visible through the former.

Also maintain a pre-filtered `mod_leds[]` subset for render step 4 efficiency.

### Reactive keypress system (GAMING_MODE)

Ring buffer of keypress events, each storing an LED index and timestamp. Filled in `process_record_user` on any key press when GAMING_MODE is active. Rendered in step 8 as a brightness-scaled fade from `theme->reactive` to off over `REACTIVE_FADE_MS` (300 ms). Buffer capacity: 8 slots. Expired slots are freed in-place during render.

### GAME layer → RGB auto-switch

In `layer_state_set_user`: when LAYER_GAME activates, save the current RGB mode and switch to the GAMING_MODE custom effect. When LAYER_GAME deactivates, restore the saved RGB mode.

---

## Debug logging

Optional `LOG(level, fmt, ...)` macro gated by a compile-time `DEBUG_LEVEL` (default: DEBUG_NONE = 0). Levels: NONE, ERROR, WARN, INFO, VERBOSE. Uses `uprintf`. Production builds compile with DEBUG_LEVEL = 0 so all LOG calls compile away. `CONSOLE_ENABLE` stays commented out in rules.mk.

---

## Commenting conventions

All five QMK hook functions (`keyboard_post_init_user`, `layer_state_set_user`, `rgb_matrix_indicators_advanced_user`, `encoder_update_user`, `process_record_user`) carry a one-line doc comment immediately above their definition describing their role in the QMK lifecycle and what this keymap uses them for.

Non-obvious range or ordering assumptions get an inline comment. The two cases to preserve:

- `is_mod_key`: `KC_LCTL through KC_RGUI covers all 8 standard modifiers in QMK keycode order`.
- `render_key_classes` digit range: `KC_0 (0x27) > KC_9 (0x26) in QMK keycode order, so KC_1..KC_0 (0x1E–0x27) covers all ten digit keycodes contiguously`.

Enums carry a brief purpose note on their typedef. COUNT / SIZE sentinel values are noted as `— do not use as index` or `— sentinel` where applicable.

Struct fields that use a sentinel value document it: `reactive_hit_t.led` notes `REACTIVE_SLOT_FREE (0xFF) when slot is empty`.

Section header blocks (`/* === ... === */`) are used for logical groupings (ENUMS, REACTIVE KEYPRESS SYSTEM, BOOT ANIMATION, THEME TABLE, RENDER PIPELINE, QMK HOOKS). Each block comment describes what belongs in it and any critical invariants (e.g., which three places must call `sync_unicode_mode`).

Do not add comments to self-evident code. Only comment where behaviour is non-obvious, a QMK API contract must be respected, or a design decision would otherwise be puzzling to a maintainer.
