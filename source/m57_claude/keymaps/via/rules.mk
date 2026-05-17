# ============================================================
# CAPS WORD
# ============================================================
# Activate: double-tap KC_LSFT (DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD in config.h).
# While active: all alpha keys are shifted; Space / Esc / punctuation deactivates it.
# LED indicator: LED 13 turns orange while Caps Word is on, red for normal Caps Lock.
#   See COLOR_CAPS_WORD in theme.h.
CAPS_WORD_ENABLE = yes


# ============================================================
# VIAL / VIA SUPPORT
# ============================================================
# Vial is a VIA fork with a live keymap editor that also covers tap-dance, combos,
# key overrides, and macros — all without reflashing.
# VIA_ENABLE is a required dependency for Vial.
VIAL_ENABLE = yes
VIA_ENABLE  = yes

# Vial RGB: exposes mode selection, brightness, and animation speed in the Vial GUI.
# Does NOT cover per-key logic — that is handled by the custom RGB pipeline in keymap.c.
VIALRGB_ENABLE = yes

# Includes rgb_matrix_user.inc at build time so the FIRMWARE_UI / DEBUG_MODE /
# GAMING_MODE custom effect stubs are compiled in.  Without this flag, custom effects
# are silently omitted even if the .inc file is present.
RGB_MATRIX_CUSTOM_USER = yes


# ============================================================
# RUNTIME-CONFIGURABLE FEATURES
# ============================================================

# Encoder Map: per-layer encoder actions defined as tables in keymap.c.
#   Layer BASE: left encoder = left/right arrow, right encoder = volume down/up.
#   Layer FN:   left encoder = RGB hue, right encoder = RGB saturation.
#   Layer NAV:  left encoder = RGB brightness, right encoder = RGB speed.
#   Layer GAME: left encoder = RGB mode prev/next.
ENCODER_MAP_ENABLE = yes

# Combo: required even with zero hardcoded combos so that Vial's runtime Combo editor
# (8 slots, VIAL_COMBO_ENTRIES in config.h) is available without reflashing.
# Storage (key_combos[]) is provided by quantum/vial.c — do not redefine COMBO_COUNT.
COMBO_ENABLE = yes

# Tap-Dance: one key → different actions on tap / hold / double-tap / tap-hold.
# Number of configurable slots: VIAL_TAP_DANCE_ENTRIES in config.h (default: 8).
# Each slot is set live in Vial's Tap-Dance editor — no reflash needed.
TAP_DANCE_ENABLE = yes

# Key Overrides: intercept any mod+key combination and send a different keycode.
# Hardcoded overrides in keymap.c:
#   RALT + [  →  å (U+00E5)
#   RALT + ;  →  ö (U+00F6)
#   RALT + '  →  ä (U+00E4)
#   Shift + Backspace  →  Delete
# Additional slots (VIAL_KEY_OVERRIDE_ENTRIES) are editable live in Vial.
KEY_OVERRIDE_ENABLE = yes

# Unicode: send å ö ä directly from firmware via OS-native input methods.
# No extra driver or software needed on any platform:
#   Windows → Alt+numpad codes  (UNICODE_MODE_WIN,   built-in)
#   Mac     → Unicode Hex Input (UNICODE_MODE_MACOS, enable once in System Prefs)
#   Linux   → Ctrl+Shift+U      (UNICODE_MODE_LINUX, built-in)
# Active method switches automatically when the OS key is pressed (see keymap.c).
UNICODE_ENABLE = yes

# Auto Shift: hold any key slightly longer than AUTO_SHIFT_TIMEOUT to produce its
# shifted variant — no Shift key needed.  Off by default after flash.
# Enable and tune the timeout live in Vial's Auto Shift panel, or assign AS_TOGG
# to any key in the Vial editor.
AUTO_SHIFT_ENABLE = yes

# Mouse Keys: move the pointer, click, and scroll from the keyboard.
# Adds KC_MS_*, KC_MS_BTN*, KC_MS_WH_* and KC_MS_ACCEL* keycodes used on NAV layer:
#   HJKL  = mouse movement (vim-style)
#   Y/U/I = left / right / middle click
#   A/S/D = speed modifier — A=fast (ACCEL2), S=medium (ACCEL1), D=sniper (ACCEL0)
# Speed tuning: MOUSEKEY_* defines in config.h (no Vial panel for raw speed values).
MOUSEKEY_ENABLE = yes

# Repeat Key: re-sends the last pressed key on demand.
# Assign QK_REPEAT_KEY / QK_ALT_REPEAT_KEY anywhere via the Vial editor.
# Useful for doubled letters (ll, tt) and arrow/scroll bursts.
REPEAT_KEY_ENABLE = yes

# Dynamic Tapping Term: adjust TAPPING_TERM live without reflashing.
# Assign DT_UP / DT_DOWN / DT_PRNT keycodes in Vial to tune the tap-vs-hold threshold.
# Affects Mod-Tap (MT), Layer-Tap (LT), and Tap-Dance simultaneously.
# Starting value is TAPPING_TERM in config.h (default: 200 ms).
DYNAMIC_TAPPING_TERM_ENABLE = yes

# Vial unlock without a physical key combo.  Appropriate for personal builds.
# Remove for shared or commercial builds where unlock security matters.
VIAL_INSECURE = yes


# ============================================================
# CONSOLE / DEBUG OUTPUT
# ============================================================
# Enables uprintf() output, readable via 'qmk console' over USB.
# Toggle on/off at runtime:  FN+D (LOG_TOGG) — state persisted to EEPROM.
# Cycle log level at runtime: Shift+FN+D — ERROR → WARN → INFO → VERBOSE.
# Flash cost: ~2 KB.  Disable only if flash space becomes critical.
CONSOLE_ENABLE = yes
