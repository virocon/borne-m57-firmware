# ============================================================
# VIAL / VIA SUPPORT
# ============================================================

VIAL_ENABLE = yes
VIA_ENABLE  = yes

ENCODER_MAP_ENABLE = yes
COMBO_ENABLE       = yes

VIAL_INSECURE = yes

# ============================================================
# CONSOLE / DEBUG OUTPUT
# ============================================================

# Uncomment to enable uprintf() output (reads via 'qmk console').
# Also set DEBUG_LEVEL in keymap.c to DEBUG_INFO or higher.
# Keep disabled for production — saves ~2 KB flash.
#CONSOLE_ENABLE = yes

# ============================================================
# VIAL RGB CONTROL (UI LAYER)
# ============================================================

VIALRGB_ENABLE = yes

# Vial RGB provides:
#   ✔ mode selection
#   ✔ brightness control
#   ✔ animation control
#
# It does NOT provide:
#   ✘ per-key logic
#   ✘ conditional behavior
#
# That logic is implemented in:
#   keymap.c (custom modes)


# ============================================================
# RGB RESPONSIBILITY MODEL
# ============================================================

# Mode selected in Vial determines behavior:
#
#   Standard mode:
#       → QMK handles LEDs
#
#   Custom mode:
#       → firmware pipeline runs
#
# User-visible result:
#
#   RGB Modes include:
#       - QMK animations
#       - FIRMWARE UI
#       - DEBUG MODE
#       - GAMING MODE