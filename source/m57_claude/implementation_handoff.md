# borne m57 — implementation handoff

This document is an instruction set for reimplementing all features and settings of the `m57_copilot-2` keymap into a fresh vial-qmk firmware tree for the same keyboard. Follow it top to bottom. All decisions, architecture choices, and implementation details are included.

---

## Hardware facts

- **MCU**: STM32F401
- **Split**: master-right (`MASTER_RIGHT`, `SPLIT_HAND_PIN C1`, `SPLIT_HAND_PIN_LOW_IS_LEFT`)
- **Transport**: full-duplex USART1 on A9/A10, 19200 baud (`SELECT_SOFT_SERIAL_SPEED 5`)
- **Matrix**: 10×7; rows 0–4 = left half, rows 5–9 = right half (single unified matrix)
- **LEDs**: 58 WS2812, 29 per side, pin A7, PWM via TIM3 ch2
- **Encoders**: left B8/B9 (index 0), right B4/B6 (index 1), resolution 2
- **Encoder LEDs**: L36 and R80 are `NO_LED` (encoder push positions, no LED)
- **VID**: `0x6401`, **PID**: `0x45D4`
- **Vial UID**: `{0x45, 0x89, 0xD8, 0xFA, 0xC7, 0x2A, 0x36, 0x89}`
- **EEPROM**: wear_leveling legacy, 4KB logical / 8KB backing
- **USB polling**: 8ms (`USB_POLLING_INTERVAL_MS 8` in keyboard-level `config.h`)

---

## File structure

All implementation lives in `keymaps/via/`:
- `keymap.c` — all logic
- `theme.h` — all color constants and theme profiles
- `rules.mk` — feature enables
- `config.h` — per-keymap defines

Do not modify `m57.c` (LED config) or `m57.h` (LAYOUT macro) unless the hardware changes.

---

## rules.mk — required enables

```makefile
VIAL_ENABLE            = yes
VIA_ENABLE             = yes
ENCODER_MAP_ENABLE     = yes
COMBO_ENABLE           = yes
VIAL_INSECURE          = yes
TAP_DANCE_ENABLE       = yes
KEY_OVERRIDE_ENABLE    = yes
UNICODE_ENABLE         = yes
AUTO_SHIFT_ENABLE      = yes
MOUSEKEY_ENABLE        = yes
REPEAT_KEY_ENABLE      = yes
DYNAMIC_TAPPING_TERM_ENABLE = yes
VIALRGB_ENABLE         = yes
```

Add informative comments explaining the purpose of each feature and how it relates to Vial editors. `CONSOLE_ENABLE` is commented out (production build).

Custom RGB modes are defined via `rgb_matrix_effects.inc` (one line per mode: `RGB_MATRIX_EFFECT(FIRMWARE_UI)` etc.). Stubs for each mode live in `keymap.c`.

---

## config.h — required defines

```c
#pragma once

#define VIAL_KEYBOARD_UID {0x45, 0x89, 0xD8, 0xFA, 0xC7, 0x2A, 0x36, 0x89}

#define COMBO_COUNT 0           // no compile-time combos; use Vial editor

#define VIAL_TAP_DANCE_ENTRIES   8
#define VIAL_COMBO_ENTRIES       8
#define VIAL_KEY_OVERRIDE_ENTRIES 8

#define UNICODE_SELECTED_MODES UNICODE_MODE_WIN, UNICODE_MODE_MACOS, UNICODE_MODE_LINUX

#define TAPPING_TERM     200
#define ONE_SHOT_TIMEOUT 1000

#define MOUSEKEY_MOVE_DELTA    8
#define MOUSEKEY_MAX_SPEED    10
#define MOUSEKEY_TIME_TO_MAX  30
#define MOUSEKEY_INTERVAL     16
```

---

## theme.h — color system

All colors are `rgb_t` structs `{r, g, b}`. No raw RGB values anywhere in `keymap.c` — everything goes through named `COLOR_*` constants.

### Color constants to define

```c
typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb_t;

// OS indicators
#define COLOR_OS_LINUX    ((rgb_t){  0,  80, 200})
#define COLOR_OS_WINDOWS  ((rgb_t){  0, 120, 255})
#define COLOR_OS_MAC      ((rgb_t){180, 180, 180})
#define COLOR_OS_ANDROID  ((rgb_t){  0, 150,   0})

// Modifiers
#define COLOR_MOD_SHIFT       ((rgb_t){255,   0,   0})
#define COLOR_MOD_CTRL        ((rgb_t){255, 120,   0})
#define COLOR_MOD_ALT         ((rgb_t){255, 200,   0})
#define COLOR_MOD_GUI         ((rgb_t){200,   0, 200})
#define COLOR_MOD_SHIFT_CTRL  ((rgb_t){255, 255,   0})
#define COLOR_MOD_ALT_GUI     ((rgb_t){255,   0, 255})

// Layer indicators
#define COLOR_LAYER_FN    ((rgb_t){255, 100,   0})
#define COLOR_LAYER_NAV   ((rgb_t){150,   0, 255})
#define COLOR_LAYER_GAME  ((rgb_t){  0, 255,   0})

// Lock states
#define COLOR_LOCK_CAPS   ((rgb_t){255,   0,   0})
#define COLOR_LOCK_NUM    ((rgb_t){  0, 200, 100})
#define COLOR_LOCK_SCRL   ((rgb_t){200, 100,   0})

// Utility
#define COLOR_WHITE       ((rgb_t){255, 255, 255})
#define COLOR_OFF         ((rgb_t){  0,   0,   0})

// Theme color slots (all theme colors reference these)
#define COLOR_BASE_UI        ((rgb_t){  0,  40, 120})
#define COLOR_LAYER1_UI      ((rgb_t){  0,   0, 255})
#define COLOR_LAYER2_UI      ((rgb_t){255,   0, 255})
#define COLOR_REACTIVE_UI    ((rgb_t){255, 200,   0})

#define COLOR_BASE_DEBUG     ((rgb_t){120,   0,   0})
#define COLOR_LAYER1_DEBUG   ((rgb_t){255,   0,   0})
#define COLOR_LAYER2_DEBUG   ((rgb_t){255, 100, 100})
#define COLOR_REACTIVE_DEBUG ((rgb_t){255, 255, 255})

#define COLOR_BASE_GAMING     ((rgb_t){  0, 120,   0})
#define COLOR_LAYER1_GAMING   ((rgb_t){  0, 255,   0})
#define COLOR_LAYER2_GAMING   ((rgb_t){255, 255,   0})
#define COLOR_REACTIVE_GAMING ((rgb_t){255, 100,   0})
```

### Theme struct and instances

```c
typedef struct {
    rgb_t base;
    rgb_t layer1;
    rgb_t layer2;
    rgb_t reactive;
} rgb_theme_t;

static const rgb_theme_t theme_ui     = { COLOR_BASE_UI,     COLOR_LAYER1_UI,     COLOR_LAYER2_UI,     COLOR_REACTIVE_UI };
static const rgb_theme_t theme_debug  = { COLOR_BASE_DEBUG,  COLOR_LAYER1_DEBUG,  COLOR_LAYER2_DEBUG,  COLOR_REACTIVE_DEBUG };
static const rgb_theme_t theme_gaming = { COLOR_BASE_GAMING, COLOR_LAYER1_GAMING, COLOR_LAYER2_GAMING, COLOR_REACTIVE_GAMING };
```

### apply_color utility

```c
static inline void apply_color(uint8_t led, rgb_t color, uint8_t brightness) {
    rgb_matrix_set_color(led,
        (uint8_t)(((uint16_t)color.r * brightness) >> 8),
        (uint8_t)(((uint16_t)color.g * brightness) >> 8),
        (uint8_t)(((uint16_t)color.b * brightness) >> 8));
}
```

---

## keymap.c — full implementation spec

### Includes

```c
#include QMK_KEYBOARD_H
#include "theme.h"
#include "eeconfig.h"
#include "print.h"
#include "host.h"
```

### Debug log system

```c
#define DEBUG_NONE 0 / DEBUG_ERROR 1 / DEBUG_WARN 2 / DEBUG_INFO 3 / DEBUG_VERBOSE 4
#ifndef DEBUG_LEVEL
#  define DEBUG_LEVEL DEBUG_NONE
#endif
#define LOG(level, fmt, ...) do { if (DEBUG_LEVEL >= level) uprintf(fmt "\n", ##__VA_ARGS__); } while(0)
```

### Enums

```c
typedef enum { LAYER_BASE=0, LAYER_FN, LAYER_NAV, LAYER_GAME } layer_id_t;
typedef enum { CUSTOM_MODE_UI=0, CUSTOM_MODE_DEBUG, CUSTOM_MODE_GAMING, CUSTOM_MODE_COUNT } custom_mode_t;
typedef enum { OS_LINUX=0, OS_WINDOWS, OS_MAC, OS_ANDROID, OS_COUNT } os_type_t;
```

### EEPROM config

```c
typedef union {
    uint32_t raw;
    struct { uint8_t os_type:8; uint8_t theme:8; };
} user_config_t;
static user_config_t user_config;
```

`eeconfig_init_user`: set `os_type = OS_WINDOWS`, `theme = CUSTOM_MODE_UI`.

`load_config_from_eeprom`: read raw, bounds-check both fields, fallback to defaults.

`save_config`: write raw to EEPROM.

### Custom keycodes

```c
enum custom_keycodes {
    OS_LINUX_SET = SAFE_RANGE,
    OS_WINDOWS_SET, OS_MAC_SET, OS_ANDROID_SET,
    THEME_NEXT, THEME_PREV
};
```

### Layer 0 — BASE

```
ESC  1    2    3    4    5                        6    7    8    9    0    BSPC
TAB  Q    W    E    R    T   [UP]    [LEFT]       Y    U    I    O    P    [
CAPS A    S    D    F    G   [DOWN]  [RIGHT]      H    J    K    L    ;    '
LSFT Z    X    C    V    B   [MUTE]  [RGB_MOD]   N    M    ,    .    /    DEL
                LALT MO(FN) SPC      ENT MO(NAV) RCTL
```

Notes:
- `[UP][DOWN]` = L16/L26 inner column (physical extra keys between halves)
- `[LEFT][RIGHT]` = R60/R70 inner column
- `[MUTE]` = L36 (encoder push), `[RGB_MOD]` = R80 (encoder push) — these have NO_LED
- `'` at R76 (was KC_NO, now KC_QUOT — needed as AltGr override trigger)

Swedish chars via AltGr (key overrides — see below):
- AltGr+`[` (R66) → å, AltGr+`;` (R75) → ö, AltGr+`'` (R76) → ä

### Layer 1 — FN

```
`    F1   F2   F3   F4   F5              F6   F7   F8   F9   F10  DEL
___  ___  ___  ___  PGUP HOME [PGUP][PGDN] F11  F12  ___  ___  ___  ]
___  TPREV ___  TNXT PGDN END  [PGDN][END]  LEFT DOWN UP   RGHT '    ___
___  ___  ___  ___  TG(GAME) BOOT [VOLD][VOLU] OS_WIN OS_MAC OS_AND OS_LNX \ ___
              ___  MO(FN) ___    ___  MO(NAV) ___
```

Notes:
- `TPREV`/`TNXT` = `THEME_PREV`/`THEME_NEXT` custom keycodes
- `TG(GAME)` activates game layer (toggle)
- `BOOT` = `QK_BOOT` (enters DFU)
- Right row 1 has 3 free slots (was PrtSc/ScrlLk/NumLk — removed, leave as `_______`)
- Encoder L (FN+non-gaming): cycle theme. Encoder R (FN+non-gaming): cycle OS

### Layer 2 — NAV/MOUSE

```
___  ___  ___  ___  ___  ___                    ___   ___      ___      ___      ___  ___
___  ___  ___  ___  PGUP HOME [WH_UP]  [WH_DN]  BTN1  BTN2     BTN3     ___      ___  ___
___  ACCEL2 ACCEL1 ACCEL0 PGDN END  [WH_DN]  [WH_UP]  MS_L  MS_D     MS_U     MS_R  ___  ___
___  ___  ___  ___  ___  ___  [___]   [___]   ___   ___      ___      ___      ___  ___
              ___  MO(FN) ___    ___  MO(NAV) ___
```

Notes:
- Left A/S/D = ACCEL2/1/0 (speed: fast/medium/sniper)
- Inner columns: scroll wheel (WH_UP/WH_DOWN)
- Right HJKL = MS_LEFT/MS_DOWN/MS_UP/MS_RGHT
- Right YUI = BTN1/BTN2/BTN3

### Layer 3 — GAME

All `_______` (transparent) except:
- Caps Lock position (row 2 col 0) = `KC_LCTL`

RGB auto-switches to GAMING_MODE on enter, restores previous mode on exit (see `layer_state_set_user`).

### Encoder map (4 layers × 2 encoders)

```c
[0] = { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
[1] = { ENCODER_CCW_CW(RGB_HUD,  RGB_HUI), ENCODER_CCW_CW(RGB_SAD,  RGB_SAI) }
[2] = { ENCODER_CCW_CW(RGB_VAD,  RGB_VAI), ENCODER_CCW_CW(RGB_SPD,  RGB_SPI) }
[3] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) }
```

### Combos

`COMBO_COUNT = 0`. No compile-time combos. Declare empty array:
```c
combo_t key_combos[COMBO_COUNT] = {};
```

### Key overrides — Swedish chars

```c
static const key_override_t ao_override = ko_make_basic(MOD_BIT(KC_RALT), KC_LBRC, UC(0x00E5)); // å
static const key_override_t oo_override = ko_make_basic(MOD_BIT(KC_RALT), KC_SCLN, UC(0x00F6)); // ö
static const key_override_t ae_override = ko_make_basic(MOD_BIT(KC_RALT), KC_QUOT, UC(0x00E4)); // ä
const key_override_t *key_overrides[] = { &ao_override, &oo_override, &ae_override, NULL };
```

### LED ownership zones

```c
static const uint8_t os_indicator_zone[]    = {0, 1, 2, 3};   // left top row
static const uint8_t layer_indicator_zone[] = {29, 30, 31, 32}; // right top row
#define CAPS_KEY_LED 13   // matrix row 2, col 0
```

Helper functions `is_os_led(led)` and `is_layer_led(led)` iterate these arrays.

### Keycode cache

```c
static uint16_t  led_keycode_cache[M57_LED_COUNT];  // M57_LED_COUNT = RGB_MATRIX_LED_COUNT or 58
static uint32_t  keycode_cache_timer = 0;
#define KEYCODE_CACHE_REFRESH_INTERVAL 100   // ms
#define MODIFIER_DIM_DIVISOR           4
#define MAX_MODIFIER_LEDS             16
static uint8_t modifier_leds[MAX_MODIFIER_LEDS];
static uint8_t modifier_led_count = 0;
```

`update_keycode_cache()`: iterate all rows/cols, call `keymap_key_to_keycode(layer, pos)` for each LED. **Never read `keymaps[][]` directly — always use this function so Vial remaps are respected.**

`build_modifier_list()`: scan cache for `KC_LCTL..KC_RGUI`, store LED indices.

### Action keys list

```c
static const uint16_t action_keys[] = {KC_ENT, KC_BSPC, KC_DEL, KC_SPC};
```

Used by `render_key_based` to highlight these keys with `theme->layer1` color.

### Reactive system

```c
#define MAX_REACTIVE         8
#define REACTIVE_DECAY_MS    300
#define REACTIVE_SLOT_UNUSED 0xFF
typedef struct { uint8_t led; uint32_t timestamp; } reactive_event_t;
static reactive_event_t reactive_buffer[MAX_REACTIVE];
static uint8_t          reactive_index = 0;
```

Records keypresses only when GAMING_MODE is active (checked in `process_record_user`).

### Boot animation

```c
#define BOOT_PHASE_DURATION        250   // ms — 4 phases = 1000ms total
#define BOOT_LED_MAX_X             210   // max x-coord in g_led_config.point[]
#define BOOT_INDICATOR_BRIGHTNESS  255
static bool     boot_indicator_active = false;
static uint32_t boot_timer = 0;
```

Set `boot_indicator_active = true` and `boot_timer = timer_read32()` at end of `keyboard_post_init_user`.

In `rgb_matrix_indicators_advanced_user`, at start, before any other rendering:
```
elapsed = timer_elapsed32(boot_timer)
if elapsed >= BOOT_PHASE_DURATION * 4 → set inactive
else:
  phase   = elapsed / BOOT_PHASE_DURATION       (0-3)
  phase_t = elapsed - phase * BOOT_PHASE_DURATION
  sweep   = (phase_t * BOOT_LED_MAX_X) / BOOT_PHASE_DURATION
  for each LED:
    lx = g_led_config.point[i].x
    phase 0: lit = (lx <= sweep)                      L→R on
    phase 1: lit = (lx >  sweep)                      L→R off
    phase 2: lit = (lx >= BOOT_LED_MAX_X - sweep)     R→L on
    phase 3: lit = (lx <  BOOT_LED_MAX_X - sweep)     R→L off
    apply_color(i, lit ? COLOR_WHITE : COLOR_OFF, BOOT_INDICATOR_BRIGHTNESS)
  return false  (skip normal pipeline during boot)
```

### Render pipeline (executed in rgb_matrix_indicators_advanced_user after boot animation)

Only runs when `rgb_matrix_get_mode() >= RGB_MATRIX_CUSTOM`. Otherwise returns `false`.

Execute in this exact order:

1. `render_flag_base` — fill all LEDs with `theme->base`; skip `is_os_led()`
2. `render_key_based` — action keys → `theme->layer1`; skip `is_os_led()`
3. `render_key_groups` — letters (A-Z), digits (1-9, 0), F1-F12 → `theme->layer2`; skip `is_os_led()`
4. `render_modifier` — modifiers dim/bright by hold state; colors by type + combo:
   - Shift: `COLOR_MOD_SHIFT` (or `COLOR_MOD_SHIFT_CTRL` if Ctrl also held)
   - Ctrl: `COLOR_MOD_CTRL` (or `COLOR_MOD_SHIFT_CTRL` if Shift also held)
   - Alt: `COLOR_MOD_ALT` (or `COLOR_MOD_ALT_GUI` if GUI also held)
   - GUI: `COLOR_MOD_GUI` (or `COLOR_MOD_ALT_GUI` if Alt also held)
   - Idle = brightness / `MODIFIER_DIM_DIVISOR`; active = full brightness
5. `render_os` — paint `os_indicator_zone[]` with OS color
6. `render_layer` — paint `layer_indicator_zone[]` with layer color; skip on LAYER_BASE
7. `render_lock_states` — Caps Lock → LED 13; Num/Scroll Lock → search keycode cache
8. `render_event` — GAMING_MODE only; reactive fade from `reactive_buffer`; skip `is_os_led()` and `is_layer_led()`

### Custom RGB effect stubs

Three stubs — each just clears to black (pipeline renders in indicators hook):
```c
bool FIRMWARE_UI(effect_params_t* params) { /* clear to COLOR_OFF */ }
bool DEBUG_MODE(effect_params_t* params)  { /* clear to COLOR_OFF */ }
bool GAMING_MODE(effect_params_t* params) { /* clear to COLOR_OFF */ }
```
Wrap in `#ifdef RGB_MATRIX_CUSTOM_USER`.

### encoder_update_user

```c
bool encoder_update_user(uint8_t index, bool clockwise) {
    layer = get_highest_layer(layer_state);
    // detect if current rgb mode is CUSTOM_MODE_GAMING
    bool in_gaming_mode = (rgb_mode >= RGB_MATRIX_CUSTOM) &&
                          ((rgb_mode - RGB_MATRIX_CUSTOM) == CUSTOM_MODE_GAMING);

    if (layer == LAYER_FN && !in_gaming_mode) {
        if (index == 0) {
            // Left encoder: cycle theme (CW=next, CCW=prev)
            // update user_config.theme % CUSTOM_MODE_COUNT
        } else {
            // Right encoder: cycle OS (CW=next, CCW=prev)
            // update user_config.os_type % OS_COUNT
            // call unicode_input_mode_set() immediately (critical — keeps Swedish chars working)
        }
        save_config();
        return false;
    }
    return true; // let encoder_map handle
}
```

**Critical:** After changing OS via encoder, call `unicode_input_mode_set()` with the matching mode — same switch statement as in `keyboard_post_init_user`. If omitted, Swedish characters break until reboot.

### process_record_user

Handle on `record->event.pressed` only.

For each `OS_*_SET` keycode:
1. Set `user_config.os_type`
2. Call `save_config()`
3. Call `unicode_input_mode_set()` with matching UNICODE_MODE_*
4. `return false`

For `THEME_NEXT` / `THEME_PREV`: cycle `user_config.theme`, save, return false.

After switch: if GAMING_MODE active, record keypress LED into `reactive_buffer`.

### keyboard_post_init_user

```c
void keyboard_post_init_user(void) {
    load_config_from_eeprom();

    // Set unicode mode from stored OS
    switch (user_config.os_type) {
        case OS_MAC:     unicode_input_mode_set(UNICODE_MODE_MACOS); break;
        case OS_LINUX:
        case OS_ANDROID: unicode_input_mode_set(UNICODE_MODE_LINUX); break;
        default:         unicode_input_mode_set(UNICODE_MODE_WIN);   break;
    }

    // Clear reactive buffer
    for (uint8_t i = 0; i < MAX_REACTIVE; i++)
        reactive_buffer[i].led = REACTIVE_SLOT_UNUSED;

    update_keycode_cache();
    build_modifier_list();

    boot_indicator_active = true;
    boot_timer = timer_read32();
}
```

### layer_state_set_user

```c
layer_state_t layer_state_set_user(layer_state_t state) {
    update_keycode_cache();
    build_modifier_list();

    // Auto-switch RGB on GAME layer toggle
    static bool    was_gaming        = false;
    static uint8_t pre_game_rgb_mode = 0;

    bool is_gaming = (state & (1u << LAYER_GAME)) != 0;
    if (is_gaming && !was_gaming) {
        pre_game_rgb_mode = rgb_matrix_get_mode();
        rgb_matrix_mode(RGB_MATRIX_CUSTOM + CUSTOM_MODE_GAMING);
    } else if (!is_gaming && was_gaming) {
        rgb_matrix_mode(pre_game_rgb_mode);
    }
    was_gaming = is_gaming;
    return state;
}
```

---

## Periodic cache refresh

Inside `rgb_matrix_indicators_advanced_user`, after boot check, before rendering:
```c
if (timer_elapsed32(keycode_cache_timer) > KEYCODE_CACHE_REFRESH_INTERVAL) {
    update_keycode_cache();
    build_modifier_list();
    keycode_cache_timer = timer_read32();
}
```
This catches Vial remaps applied at runtime without requiring a layer change.

---

## Key decisions to preserve

| Decision | Reason |
|---|---|
| `apply_color` uses `(color * brightness) >> 8` not `/255` | Cheaper on MCU; 0.4% difference at max brightness is imperceptible |
| Keycode cache uses `keymap_key_to_keycode()`, never `keymaps[][]` | Respects Vial runtime remapping |
| Reactive buffer only filled in GAMING_MODE | Avoids wasted buffer slots in other modes |
| OS indicator zone excluded from ALL render steps except `render_os` | Prevents any theme color from bleeding into the OS indicator |
| Layer indicator zone excluded from `render_event` | Prevents reactive flashes on indicator LEDs |
| Unicode mode set in 3 places: boot, key press, encoder | All OS-change paths must stay in sync or Swedish chars break on the changed OS |
| `COMBO_COUNT 0` with empty array | Keeps COMBO_ENABLE for Vial runtime combos without any compile-time combos |
| GAME layer only changes Caps→Ctrl | Minimal diff — all other keys transparent to BASE |
| Encoder FN+GAMING_MODE → encoder_map handles (no theme/OS interception) | Prevents accidental OS/theme changes during gaming |

---

## Things that are NOT in this firmware (intentionally excluded)

- Home row mods (MT on A/S/D/F etc.) — not requested
- Achordion — not requested
- Polish characters — decided against, Swedish only
- EurKEY OS layout — decided against (requires admin on Windows)
- WinCompose — decided against (requires install)
- Numpad layer — removed, replaced by mouse layer
- J+K→Esc combo — removed (ESC already on L00)
