# borne m57 — `via` keymap

Vial-QMK firmware for the **borne m57** split keyboard (STM32F401, 58 WS2812 LEDs, 10×7 matrix).

---

## Table of contents

1. [Building and flashing](#building-and-flashing)
2. [Layer overview](#layer-overview)
3. [Swedish characters](#swedish-characters)
4. [OS detection](#os-detection)
5. [RGB themes](#rgb-themes)
6. [RGB indicators](#rgb-indicators)
7. [Boot animation](#boot-animation)
8. [Encoders](#encoders)
9. [Mouse keys and sniper mode](#mouse-keys-and-sniper-mode)
10. [Combos](#combos)
11. [Vial live editors](#vial-live-editors)
12. [Timing and tapping](#timing-and-tapping)
13. [Auto Shift](#auto-shift)
14. [Repeat Key](#repeat-key)
15. [USB polling](#usb-polling)
16. [EEPROM storage](#eeprom-storage)
17. [Debug logging](#debug-logging)

---

## Building and flashing

```
make m57:via
```

Output format: `uf2` (`UF2_FAMILY = 0xabcdf401`).
Flash by entering the bootloader (FN + B), then drag-and-drop the `.uf2` file onto the mass-storage device that appears.

---

## Layer overview

The keyboard has 4 compiled layers (0–3). Layers 4–9 are reserved for Vial runtime remapping.

| # | Name | Activation | Purpose |
|---|---|---|---|
| 0 | **BASE** | always | Standard QWERTY typing |
| 1 | **FN** | hold left thumb | F-keys, navigation, system, OS selection |
| 2 | **NAV / MOUSE** | hold right thumb | Mouse movement, scroll, page navigation |
| 3 | **GAME** | toggle FN+V | Gaming layout, reactive RGB |

### Layer 0 — BASE

Standard US QWERTY. Notable assignments:

| Key | Position | Note |
|---|---|---|
| `MO(FN)` | left thumb middle | hold for FN layer |
| `MO(NAV)` | right thumb middle | hold for NAV/MOUSE layer |
| `[` | R66 (after P) | AltGr+[ → **å** |
| `;` | R75 (after L) | AltGr+; → **ö** |
| `'` | R76 (after ;) | AltGr+' → **ä** |
| inner col L | L16/L26 | cursor Up/Down |
| inner col R | R60/R70 | cursor Left/Right |
| encoder L push | L36 | Mute |
| encoder R push | R80 | RGB mode cycle |

### Layer 1 — FN

| Area | Keys |
|---|---|
| Top row | `` ` ``, F1–F10, Del |
| Row 1 right | F11, F12, *(3 free slots)*, `]` |
| Row 2 right | ← ↓ ↑ → (arrow keys), `'`, *(free)* |
| Row 3 left | TG(GAME) on V, QK_BOOT on B |
| Row 3 right | OS_WIN, OS_MAC, OS_ANDROID, OS_LINUX, `\` |
| Encoder push L | Vol↓ |
| Encoder push R | Vol↑ |
| Encoder L rotate (FN+UI) | Theme cycle |
| Encoder R rotate (FN+UI) | OS cycle |

### Layer 2 — NAV / MOUSE

Two-hand mouse control: left hand sets speed, right hand moves.

| Left hand | Key | Right hand | Key |
|---|---|---|---|
| A | ACCEL2 — fast | H | Mouse ← |
| S | ACCEL1 — medium | J | Mouse ↓ |
| D | ACCEL0 — sniper (slowest) | K | Mouse ↑ |
| R | Page Up | L | Mouse → |
| T | Home | Y | Left click |
| F | Page Down | U | Right click |
| G | End | I | Middle click |
| inner col L (row1) | Scroll Up | inner col R (row1) | Scroll Down |
| inner col L (row2) | Scroll Down | inner col R (row2) | Scroll Up |

### Layer 3 — GAME

Identical to BASE except:
- **Caps Lock → Left Ctrl** (standard gaming remap)
- RGB automatically switches to GAMING_MODE on enter; restores previous mode on exit

---

## Swedish characters

No OS keyboard layout change needed. Works on any OS with standard US layout.

| Press | Output | Unicode |
|---|---|---|
| AltGr + `[` | **å** | U+00E5 |
| AltGr + `;` | **ö** | U+00F6 |
| AltGr + `'` | **ä** | U+00E4 |

`AltGr` = Right Alt key.

### Per-OS setup (one time only)

| OS | What to do | Admin? |
|---|---|---|
| **Windows** | Nothing — uses built-in Alt+numpad method | ❌ No |
| **Mac** | System Settings → Keyboard → Input Sources → add **Unicode Hex Input** | ❌ No |
| **Linux** | Nothing — uses built-in Ctrl+Shift+U method | ❌ No |

The firmware automatically selects the correct method based on the stored OS setting. It switches immediately when the OS key is pressed or the OS is changed via the right encoder on FN.

---

## OS detection

The keyboard stores the active OS in EEPROM and uses it to:
- Color the **left top-row LEDs 0–3** as an OS indicator
- Select the correct Unicode input method for Swedish characters
- (Future: adapt modifier key behavior per OS)

### Changing OS

**Method 1 — Key press** (FN layer, row 3 right):

| Key | OS |
|---|---|
| N position | Windows |
| M position | macOS |
| `,` position | Android |
| `.` position | Linux |

**Method 2 — Right encoder** (hold FN, rotate right encoder):
Cycles through all 4 OS options. Visual feedback: left indicator LEDs change color immediately.

| OS | LED color |
|---|---|
| Windows | Bright blue |
| macOS | Neutral gray |
| Linux | Cyan-blue |
| Android | Green |

Default OS after first flash: **Windows**.

---

## RGB themes

Three built-in themes control the custom RGB pipeline colors.

| Theme | Base | Action keys | Letters/digits | Reactive |
|---|---|---|---|---|
| **UI** | Dim blue | Pure blue | Magenta | Warm yellow |
| **Debug** | Dark red | Bright red | Soft red | White |
| **Gaming** | Green | Bright green | Yellow | Orange |

### Changing theme

**Method 1 — Key press** (FN layer, row 2 left):
- `A` position → THEME_PREV
- `D` position → THEME_NEXT

**Method 2 — Left encoder** (hold FN, rotate left encoder):
Cycles forward/backward through themes.

Theme is stored in EEPROM and persists across power cycles.

### Standard QMK RGB modes

When Vial selects any standard QMK animation (e.g., Rainbow, Breathing), the custom pipeline is bypassed entirely and QMK controls the LEDs directly. Switch back to a custom mode (FIRMWARE UI / DEBUG MODE / GAMING MODE) in Vial to re-enable the pipeline.

---

## RGB indicators

All indicators are painted on top of the active theme. They are always visible regardless of brightness setting.

### OS indicator (left top row, LEDs 0–3)

Shows active OS. Colors described in [OS detection](#os-detection).

### Layer indicator (right top row, LEDs 29–32)

| Layer | Color |
|---|---|
| BASE | *(theme base color — no override)* |
| FN | Orange |
| NAV/MOUSE | Purple |
| GAME | Green |

### Lock state indicators

| Lock | LED | Color |
|---|---|---|
| Caps Lock | LED 13 (Caps key) | Red |
| Num Lock | Key assigned KC_NUM in current layer | Teal |
| Scroll Lock | Key assigned KC_SCRL in current layer | Amber |

### Modifier indicators

Modifier keys are dimmed to 25% when idle and full brightness when held. Colors change when two modifiers are held simultaneously:

| Modifier | Idle/active color | Combo color |
|---|---|---|
| Shift | Red | Yellow (with Ctrl) |
| Ctrl | Orange | Yellow (with Shift) |
| Alt | Yellow | Bright magenta (with GUI) |
| GUI/Win | Magenta | Bright magenta (with Alt) |

---

## Boot animation

On power-up the keyboard plays a 1-second curtain sweep animation before entering normal operation.

**Sequence:**
1. Columns light up left → right (curtain closes from left)
2. Columns go dark left → right (curtain opens to right)
3. Columns light up right → left (curtain closes from right)
4. Columns go dark right → left (curtain opens to left)

Each phase: 250 ms. Total: 1 000 ms. Color: white at full brightness.

**Tuning** (in `keymap.c`):
```c
#define BOOT_PHASE_DURATION   250   // ms per phase
#define BOOT_LED_MAX_X        210   // max x-coordinate in LED config
#define BOOT_INDICATOR_BRIGHTNESS 255
```

---

## Encoders

The keyboard has two rotary encoders with push-button functionality.

| Encoder | Push | Normal rotation | FN layer (non-gaming) |
|---|---|---|---|
| **Left** (B8/B9) | Mute | CCW=→ / CW=← (cursor) | Rotate = theme cycle |
| **Right** (B4/B6) | RGB mode cycle | CCW=Vol↓ / CW=Vol↑ | Rotate = OS cycle |

In GAME layer the encoder_map controls RGB parameters (Hue, Saturation, Value, Speed) instead of theme/OS.

---

## Mouse keys and sniper mode

Mouse movement is on NAV layer (hold right thumb). See [Layer 2](#layer-2--nav--mouse) for full key map.

**Speed modes** (hold with left hand while moving with right):

| Key | Mode | Behavior |
|---|---|---|
| *(nothing held)* | Accelerated | Starts at 8px/step, ramps to 80px/step over ~480ms |
| D = ACCEL0 | Sniper | Constant 8px/step — precise targeting |
| S = ACCEL1 | Medium | Constant mid-range speed |
| A = ACCEL2 | Fast | Jumps immediately to maximum speed |

**Speed tuning** (in `keymaps/via/config.h` — requires reflash):
```c
#define MOUSEKEY_MOVE_DELTA    8   // px per step at minimum
#define MOUSEKEY_MAX_SPEED    10   // multiplier ceiling (10 × 8 = 80px max)
#define MOUSEKEY_TIME_TO_MAX  30   // steps to reach max (~480ms)
#define MOUSEKEY_INTERVAL     16   // ms between steps (~60fps)
```

---

## Combos

No compile-time combos are defined. Use Vial's Combo editor (8 runtime slots) to add combos without reflashing — no rebuild needed.

---

## Vial live editors

The following editors are available in Vial without reflashing:

| Editor | Vial location | Notes |
|---|---|---|
| **Keymap** | main view | All 10 layers editable |
| **Tap-Dance** | Tap-Dance tab | 8 slots; tap/hold/double-tap/tap-hold per key |
| **Combos** | Combos tab | 8 slots; coexists with compiled J+K combo |
| **Key Overrides** | Key Override tab | 8 slots; note: Swedish chars use compile-time overrides |
| **Auto Shift** | Auto Shift tab | Enable/disable + timeout slider |
| **Macros** | Macros tab | 15 dynamic macros |
| **Quantum keycodes** | Quantum tab | DT_UP / DT_DOWN / DT_PRNT — tapping term tuning |

**Important:** Keymaps edited in Vial are stored in EEPROM and survive reflash. Compiled layer definitions serve as factory defaults (loaded only on first flash or after EEPROM reset).

---

## Timing and tapping

| Setting | Value | File | Tunable at runtime? |
|---|---|---|---|
| `TAPPING_TERM` | 200 ms | `config.h` | ✅ via DT_UP/DT_DOWN keycodes |
| `ONE_SHOT_TIMEOUT` | 1000 ms | `config.h` | ❌ reflash |

**DT_UP / DT_DOWN / DT_PRNT** — assign anywhere in Vial (Quantum tab).
Each press of DT_UP adds 5ms to TAPPING_TERM. DT_PRNT prints the current value to the console.

Affects: Mod-Tap (MT), Layer-Tap (LT), Tap-Dance simultaneously.

---

## Auto Shift

Hold any key slightly longer than the Auto Shift timeout to get its shifted character — no Shift key needed.

- **Off by default** after flash — does not activate until explicitly enabled
- Enable/disable: Vial → Auto Shift tab, or assign AS_ON / AS_OFF / AS_TOGG keycodes
- Timeout: adjustable in Vial → Auto Shift tab (no reflash needed)

---

## Repeat Key

Assign `QK_REPEAT_KEY` or `QK_ALT_REPEAT_KEY` to any key via Vial.
- **Repeat Key**: re-sends the last pressed key (useful for `ll`, `tt`, arrow bursts)
- **Alt Repeat Key**: sends the "opposite" of the last key (e.g., `(` → `)`)

---

## USB polling

USB HID polling interval: **8 ms** (~125 Hz).

Set in `config.h`:
```c
#define USB_POLLING_INTERVAL_MS 8
```

| Interval | Latency | Power |
|---|---|---|
| 1 ms | Lowest | Highest (MCU rarely sleeps) |
| **8 ms** | ±8 ms (imperceptible for typing) | Balanced ✓ |
| 125 ms | High | Lowest |

---

## EEPROM storage

The keyboard stores two values in EEPROM (survives power cycles):

| Value | Default | How to reset |
|---|---|---|
| Active OS | Windows | Flash new firmware or hold QK_BOOT |
| Active theme | UI | — |

EEPROM driver: `wear_leveling` (legacy), 4 KB logical / 8 KB backing.
Max dynamic keymap address: 4095 bytes (10 layers × 15 macros within this budget).

---

## Debug logging

Silent by default. To enable:

1. In `rules.mk`, uncomment: `CONSOLE_ENABLE = yes`
2. In `keymap.c`, change: `#define DEBUG_LEVEL DEBUG_INFO` (or `DEBUG_VERBOSE`)
3. Read output with: `qmk console`

Log levels: `DEBUG_NONE`, `DEBUG_ERROR`, `DEBUG_WARN`, `DEBUG_INFO`, `DEBUG_VERBOSE`

Keep `CONSOLE_ENABLE` disabled for production — saves ~2 KB of flash.
