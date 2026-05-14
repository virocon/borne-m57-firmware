// Custom RGB matrix effect definitions for M57 firmware.
// Included by QMK's RGB matrix system when RGB_MATRIX_CUSTOM_USER = yes.
// Each name here gets a corresponding implementation in keymap.c.
// The actual per-LED rendering is handled by rgb_matrix_indicators_advanced_user();
// these effect stubs only clear the canvas so the indicator hook paints cleanly.
RGB_MATRIX_EFFECT(FIRMWARE_UI)
RGB_MATRIX_EFFECT(DEBUG_MODE)
RGB_MATRIX_EFFECT(GAMING_MODE)
