#include QMK_KEYBOARD_H
#include "theme.h"
#include "eeconfig.h"
#include "print.h"


// ============================================================
// DEBUG LOG SYSTEM
// ============================================================

#define DEBUG_NONE    0
#define DEBUG_ERROR   1
#define DEBUG_WARN    2
#define DEBUG_INFO    3
#define DEBUG_VERBOSE 4

#ifndef DEBUG_LEVEL
#  define DEBUG_LEVEL DEBUG_NONE  // silent by default; set to DEBUG_INFO or higher
#endif                            // and enable CONSOLE_ENABLE = yes in rules.mk

#define LOG(level, fmt, ...) \
    do { if (DEBUG_LEVEL >= level) uprintf(fmt "\n", ##__VA_ARGS__); } while (0)


// ============================================================
// LAYER NAMES
// ============================================================

typedef enum {
    LAYER_BASE = 0,
    LAYER_FN,
    LAYER_NAV,
    LAYER_GAME
} layer_id_t;


// ============================================================
// CUSTOM MODE IDENTIFIERS
// ============================================================

/*
 * Mode = behavior (what the render pipeline does)
 * Theme = colors (which rgb_theme_t profile is active)
 *
 * Decoupled intentionally: same mode can use different themes
 * and vice versa.
 */

typedef enum {
    CUSTOM_MODE_UI     = 0,
    CUSTOM_MODE_DEBUG,
    CUSTOM_MODE_GAMING,
    CUSTOM_MODE_COUNT
} custom_mode_t;

static custom_mode_t current_custom_mode = CUSTOM_MODE_UI;


// ============================================================
// OS STATE
// ============================================================

typedef enum {
    OS_LINUX   = 0,
    OS_WINDOWS,
    OS_MAC,
    OS_ANDROID,
    OS_COUNT
} os_type_t;


// ============================================================
// EEPROM USER CONFIG  (packed into one uint32_t for eeconfig API)
// ============================================================

typedef union {
    uint32_t raw;
    struct {
        uint8_t os_type : 8;
        uint8_t theme   : 8;
    };
} user_config_t;

static user_config_t user_config;

void eeconfig_init_user(void) {
    user_config.os_type = OS_WINDOWS;
    user_config.theme   = CUSTOM_MODE_UI;
    eeconfig_update_user(user_config.raw);
}


// ============================================================
// OS + THEME KEYCODES
// ============================================================

enum custom_keycodes {
    OS_LINUX_SET = SAFE_RANGE,
    OS_WINDOWS_SET,
    OS_MAC_SET,
    OS_ANDROID_SET,
    THEME_NEXT,
    THEME_PREV
};


// ============================================================
// KEYMAPS  (Vial can remap layers 4–9 at runtime)
// ============================================================

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = LAYOUT(
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,                       KC_6,  KC_7,   KC_8,    KC_9,   KC_0,    KC_BSPC,
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,  KC_UP,      KC_LEFT, KC_Y,  KC_U,   KC_I,    KC_O,   KC_P,    KC_LBRC,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,  KC_DOWN,    KC_RIGHT,KC_H,  KC_J,   KC_K,    KC_L,   KC_SCLN, KC_ENTER,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,  KC_MUTE,    RGB_MOD, KC_N,  KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_DEL,
   //|-------+--------+-------+--------+---------+------+---.  .---+--------+------+-------+--------+-------+---------+---------|
                        KC_LALT, MO(1), KC_SPC,                     KC_ENT,  MO(2), KC_RCTL
                  //`-----------------------------------------'  `-----------------------------------'
  ),

  [1] = LAYOUT(
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_ESC,  KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,                      KC_F6, KC_F7,  KC_F8,   KC_F9,  KC_F10,  KC_BSPC,
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,  KC_UP,      KC_LEFT, KC_Y,  KC_U,   KC_I,    KC_O,   KC_P,    KC_LBRC,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,  KC_DOWN,    KC_RIGHT,KC_H,  KC_J,   KC_K,    KC_L,   KC_SCLN, KC_ENTER,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,  KC_MUTE,    RGB_MOD, KC_N,  KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_DEL,
   //|-------+--------+-------+--------+---------+------+---.  .---+--------+------+-------+--------+-------+---------+---------|
                        KC_LCTL, MO(1), KC_SPC,                     KC_ENT,  MO(2), KC_RCTL
  ),

  [2] = LAYOUT(
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,                       KC_6,  KC_7,   KC_8,    KC_9,   KC_0,    KC_BSPC,
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,  KC_UP,      KC_LEFT, KC_Y,  KC_U,   KC_I,    KC_O,   KC_P,    KC_LBRC,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,  KC_DOWN,    KC_RIGHT,KC_H,  KC_J,   KC_K,    KC_L,   KC_SCLN, KC_ENTER,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,  KC_MUTE,    RGB_MOD, KC_N,  KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_DEL,
   //|-------+--------+-------+--------+---------+------+---.  .---+--------+------+-------+--------+-------+---------+---------|
                        KC_LCTL, MO(1), KC_SPC,                     KC_ENT,  MO(2), KC_RCTL
  ),

  [3] = LAYOUT(
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,                       KC_6,  KC_7,   KC_8,    KC_9,   KC_0,    KC_BSPC,
   //,-------------------------------------------------------.     ,-------------------------------------------------------------.
     KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,  KC_UP,      KC_LEFT, KC_Y,  KC_U,   KC_I,    KC_O,   KC_P,    KC_LBRC,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,  KC_DOWN,    KC_RIGHT,KC_H,  KC_J,   KC_K,    KC_L,   KC_SCLN, KC_ENTER,
   //|-------+--------+--------+-------+--------+-------+----|  |---+--------+------+-------+--------+-------+---------+---------|
     KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,  KC_MUTE,    RGB_MOD, KC_N,  KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_DEL,
   //|-------+--------+-------+--------+---------+------+---.  .---+--------+------+-------+--------+-------+---------+---------|
                        KC_LCTL, MO(1), KC_SPC,                     KC_ENT,  MO(2), KC_RCTL
  )
};


// ============================================================
// ENCODER MAP
// ============================================================

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT),  ENCODER_CCW_CW(KC_VOLD,  KC_VOLU) },
    [1] = { ENCODER_CCW_CW(RGB_HUD,  RGB_HUI),  ENCODER_CCW_CW(RGB_SAD,  RGB_SAI) },
    [2] = { ENCODER_CCW_CW(RGB_VAD,  RGB_VAI),  ENCODER_CCW_CW(RGB_SPD,  RGB_SPI) },
    [3] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),  ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
};
#endif


// ============================================================
// LED OWNERSHIP  (OS indicator zone — never overwritten by other layers)
// ============================================================

static const uint8_t os_indicator_zone[] = {0, 1, 2, 3};
#define OS_INDICATOR_LED_COUNT (sizeof(os_indicator_zone) / sizeof(os_indicator_zone[0]))

// Action keys rendered with theme->layer1 (edit this list to change which keys are highlighted)
static const uint16_t action_keys[] = {KC_ENT, KC_BSPC, KC_DEL, KC_SPC};
#define ACTION_KEY_COUNT (sizeof(action_keys) / sizeof(action_keys[0]))

static bool is_os_led(uint8_t led) {
    for (uint8_t i = 0; i < OS_INDICATOR_LED_COUNT; i++) {
        if (os_indicator_zone[i] == led) return true;
    }
    return false;
}


// ============================================================
// KEYCODE CACHE  (rebuilt on layer change; refreshed on timer)
// ============================================================

#ifdef RGB_MATRIX_LED_COUNT
#  define M57_LED_COUNT RGB_MATRIX_LED_COUNT
#else
#  define M57_LED_COUNT 58
#endif

static uint16_t  led_keycode_cache[M57_LED_COUNT];
static uint32_t  keycode_cache_timer = 0;
#define KEYCODE_CACHE_REFRESH_INTERVAL 100
#define MODIFIER_DIM_DIVISOR           4

#define MAX_MODIFIER_LEDS 16
static uint8_t modifier_leds[MAX_MODIFIER_LEDS];
static uint8_t modifier_led_count = 0;


// ============================================================
// REACTIVE SYSTEM  (keypress fade-out, used in GAMING MODE)
// ============================================================

#define MAX_REACTIVE         8
#define REACTIVE_DECAY_MS    300
#define REACTIVE_SLOT_UNUSED 0xFF

typedef struct {
    uint8_t  led;       // 0xFF = slot unused
    uint32_t timestamp;
} reactive_event_t;

static reactive_event_t reactive_buffer[MAX_REACTIVE];
static uint8_t          reactive_index = 0;


// ============================================================
// BOOT INDICATOR
// ============================================================

#define BOOT_INDICATOR_DURATION   600
#define BOOT_INDICATOR_BRIGHTNESS 255

static bool     boot_indicator_active = false;
static uint32_t boot_timer            = 0;


// ============================================================
// EEPROM HELPERS
// ============================================================

static void load_config_from_eeprom(void) {
    user_config.raw = eeconfig_read_user();

    if (user_config.os_type >= OS_COUNT) {
        user_config.os_type = OS_WINDOWS;
    }
    if (user_config.theme >= CUSTOM_MODE_COUNT) {
        user_config.theme = CUSTOM_MODE_UI;
    }

    LOG(DEBUG_INFO, "[EEPROM] os=%d theme=%d", user_config.os_type, user_config.theme);
}

static void save_config(void) {
    eeconfig_update_user(user_config.raw);
    LOG(DEBUG_INFO, "[CFG] saved os=%d theme=%d", user_config.os_type, user_config.theme);
}


// ============================================================
// THEME SELECTION
// ============================================================

static const rgb_theme_t* const themes[CUSTOM_MODE_COUNT] = {
    [CUSTOM_MODE_UI]     = &theme_ui,
    [CUSTOM_MODE_DEBUG]  = &theme_debug,
    [CUSTOM_MODE_GAMING] = &theme_gaming,
};

static const rgb_theme_t* get_active_theme(void) {
    if (user_config.theme < CUSTOM_MODE_COUNT) return themes[user_config.theme];
    LOG(DEBUG_WARN, "[THEME] invalid index=%d, fallback UI", user_config.theme);
    return &theme_ui;
}


// ============================================================
// CACHE HELPERS
// ============================================================

/*
 * All key lookups use keymap_key_to_keycode() so Vial remapping
 * is respected — never read keymaps[][] directly.
 */

static inline bool is_modifier_keycode(uint16_t kc) {
    return (kc >= KC_LCTL && kc <= KC_RGUI);
}

static void update_keycode_cache(void) {
    uint8_t layer = get_highest_layer(layer_state);
    LOG(DEBUG_VERBOSE, "[CACHE] update layer=%d", layer);

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led != NO_LED) {
                led_keycode_cache[led] =
                    keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col});
            }
        }
    }
}

static void build_modifier_list(void) {
    modifier_led_count = 0;
    for (uint8_t i = 0; i < M57_LED_COUNT && modifier_led_count < MAX_MODIFIER_LEDS; i++) {
        if (is_modifier_keycode(led_keycode_cache[i])) {
            modifier_leds[modifier_led_count++] = i;
        }
    }
    LOG(DEBUG_INFO, "[MOD] modifier_count=%d", modifier_led_count);
}


// ============================================================
// RENDER PIPELINE
// ============================================================

// Step 1: base color from active theme — OS zone excluded.
static void render_flag_base(uint8_t led_min, uint8_t led_max, const rgb_theme_t* theme, uint8_t brightness) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (is_os_led(i)) continue;
        apply_color(i, theme->base, brightness);
    }
}

// Step 2: action keys highlighted with theme->layer1 (list defined in action_keys[]).
static void render_key_based(uint8_t led_min, uint8_t led_max, const rgb_theme_t* theme, uint8_t brightness) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (is_os_led(i)) continue;
        uint16_t kc = led_keycode_cache[i];
        for (uint8_t j = 0; j < ACTION_KEY_COUNT; j++) {
            if (kc == action_keys[j]) {
                apply_color(i, theme->layer1, brightness);
                break;
            }
        }
    }
}

// Step 3: key groups (letters, digits, function keys) with theme->layer2.
static void render_key_groups(uint8_t led_min, uint8_t led_max, const rgb_theme_t* theme, uint8_t brightness) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (is_os_led(i)) continue;
        uint16_t kc = led_keycode_cache[i];
        if ((kc >= KC_A  && kc <= KC_Z)   ||
            (kc >= KC_1  && kc <= KC_9)   ||
             kc == KC_0                   ||
            (kc >= KC_F1 && kc <= KC_F12)) {
            apply_color(i, theme->layer2, brightness);
        }
    }
}

// Step 4: modifier LEDs — colored by type, dim when idle, bright when held.
static void render_modifier(uint8_t led_min, uint8_t led_max, uint8_t brightness) {
    uint8_t mods = get_mods() | get_oneshot_mods();

    for (uint8_t i = 0; i < modifier_led_count; i++) {
        uint8_t  led = modifier_leds[i];
        if (led < led_min || led >= led_max) continue;
        if (is_os_led(led)) continue;

        uint16_t kc = led_keycode_cache[led];
        rgb_t    color;
        bool     active;

        switch (kc) {
            case KC_LSFT: case KC_RSFT:
                color  = (mods & MOD_MASK_CTRL)  ? COLOR_MOD_SHIFT_CTRL : COLOR_MOD_SHIFT;
                active = (mods & MOD_MASK_SHIFT) != 0; break;
            case KC_LCTL: case KC_RCTL:
                color  = (mods & MOD_MASK_SHIFT) ? COLOR_MOD_SHIFT_CTRL : COLOR_MOD_CTRL;
                active = (mods & MOD_MASK_CTRL)  != 0; break;
            case KC_LALT: case KC_RALT:
                color  = (mods & MOD_MASK_GUI)   ? COLOR_MOD_ALT_GUI : COLOR_MOD_ALT;
                active = (mods & MOD_MASK_ALT)   != 0; break;
            case KC_LGUI: case KC_RGUI:
                color  = (mods & MOD_MASK_ALT)   ? COLOR_MOD_ALT_GUI : COLOR_MOD_GUI;
                active = (mods & MOD_MASK_GUI)   != 0; break;
            default: continue;
        }
        apply_color(led, color, active ? brightness : brightness / MODIFIER_DIM_DIVISOR);
    }
}

// Step 5: OS indicator zone — always painted last in base pipeline.
static void render_os(uint8_t led_min, uint8_t led_max, uint8_t brightness) {
    rgb_t color;
    switch ((os_type_t)user_config.os_type) {
        case OS_LINUX:   color = COLOR_OS_LINUX;   break;
        case OS_WINDOWS: color = COLOR_OS_WINDOWS; break;
        case OS_MAC:     color = COLOR_OS_MAC;     break;
        case OS_ANDROID: color = COLOR_OS_ANDROID; break;
        default:         color = COLOR_OFF;        break;
    }
    for (uint8_t i = 0; i < OS_INDICATOR_LED_COUNT; i++) {
        uint8_t led = os_indicator_zone[i];
        if (led >= led_min && led < led_max) {
            apply_color(led, color, brightness);
        }
    }
}

// Step 6 (GAMING MODE only): reactive fade-out on keypress.
static void render_event(uint8_t led_min, uint8_t led_max, const rgb_theme_t* theme, uint8_t brightness) {
    for (uint8_t i = 0; i < MAX_REACTIVE; i++) {
        if (reactive_buffer[i].led == REACTIVE_SLOT_UNUSED) continue;

        uint32_t elapsed = timer_elapsed32(reactive_buffer[i].timestamp);
        if (elapsed >= REACTIVE_DECAY_MS) {
            reactive_buffer[i].led = REACTIVE_SLOT_UNUSED;
            continue;
        }

        uint8_t led = reactive_buffer[i].led;
        if (led < led_min || led >= led_max) continue;
        if (is_os_led(led)) continue;

        uint8_t intensity = (uint8_t)(
            ((uint32_t)(REACTIVE_DECAY_MS - elapsed) * brightness) / REACTIVE_DECAY_MS
        );
        apply_color(led, theme->reactive, intensity);
    }
}


// ============================================================
// CUSTOM RGB EFFECT STUBS  (canvas clear — indicators hook renders)
// ============================================================

#ifdef RGB_MATRIX_CUSTOM_USER

bool FIRMWARE_UI(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) rgb_matrix_set_color(i, 0, 0, 0);
    return led_max < RGB_MATRIX_LED_COUNT;
}

bool DEBUG_MODE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) rgb_matrix_set_color(i, 0, 0, 0);
    return led_max < RGB_MATRIX_LED_COUNT;
}

bool GAMING_MODE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) rgb_matrix_set_color(i, 0, 0, 0);
    return led_max < RGB_MATRIX_LED_COUNT;
}

#endif


// ============================================================
// INPUT HOOKS
// ============================================================

bool encoder_update_user(uint8_t index, bool clockwise) {
    // FN layer (1) + encoder → cycle OS
    if (get_highest_layer(layer_state) == LAYER_FN) {
        if (clockwise) {
            user_config.os_type = (user_config.os_type + 1) % OS_COUNT;
        } else {
            user_config.os_type = (user_config.os_type == 0)
                ? OS_COUNT - 1
                : user_config.os_type - 1;
        }
        LOG(DEBUG_INFO, "[ENC] OS=%d", user_config.os_type);
        save_config();
        return false;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
        case OS_LINUX_SET:
            user_config.os_type = OS_LINUX;
            save_config();
            LOG(DEBUG_INFO, "[KEY] OS=LINUX");
            return false;
        case OS_WINDOWS_SET:
            user_config.os_type = OS_WINDOWS;
            save_config();
            LOG(DEBUG_INFO, "[KEY] OS=WINDOWS");
            return false;
        case OS_MAC_SET:
            user_config.os_type = OS_MAC;
            save_config();
            LOG(DEBUG_INFO, "[KEY] OS=MAC");
            return false;
        case OS_ANDROID_SET:
            user_config.os_type = OS_ANDROID;
            save_config();
            LOG(DEBUG_INFO, "[KEY] OS=ANDROID");
            return false;

        case THEME_NEXT:
            user_config.theme = (user_config.theme + 1) % CUSTOM_MODE_COUNT;
            save_config();
            LOG(DEBUG_INFO, "[KEY] THEME_NEXT=%d", user_config.theme);
            return false;
        case THEME_PREV:
            user_config.theme = (user_config.theme == 0)
                ? CUSTOM_MODE_COUNT - 1
                : user_config.theme - 1;
            save_config();
            LOG(DEBUG_INFO, "[KEY] THEME_PREV=%d", user_config.theme);
            return false;
    }

    // Record keypress LED for reactive system
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint8_t led = g_led_config.matrix_co[row][col];
    if (led != NO_LED) {
        LOG(DEBUG_VERBOSE, "[REACT] LED=%d", led);
        reactive_buffer[reactive_index].led       = led;
        reactive_buffer[reactive_index].timestamp = timer_read32();
        reactive_index = (reactive_index + 1) % MAX_REACTIVE;
    }

    return true;
}


// ============================================================
// QMK HOOKS
// ============================================================

void keyboard_post_init_user(void) {
    load_config_from_eeprom();

    for (uint8_t i = 0; i < MAX_REACTIVE; i++) {
        reactive_buffer[i].led = REACTIVE_SLOT_UNUSED;
    }

    update_keycode_cache();
    build_modifier_list();

    boot_indicator_active = true;
    boot_timer            = timer_read32();

    LOG(DEBUG_INFO, "[INIT] leds=%u mods=%u os=%d theme=%d",
        (uint8_t)M57_LED_COUNT, modifier_led_count,
        user_config.os_type, user_config.theme);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    update_keycode_cache();
    build_modifier_list();
    LOG(DEBUG_VERBOSE, "[LAYER] active=%u", get_highest_layer(state));
    return state;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Boot flash: all white for BOOT_INDICATOR_DURATION ms
    if (boot_indicator_active) {
        if (timer_elapsed32(boot_timer) < BOOT_INDICATOR_DURATION) {
            for (uint8_t i = led_min; i < led_max; i++) {
                apply_color(i, COLOR_WHITE, BOOT_INDICATOR_BRIGHTNESS);
            }
            return false;
        }
        boot_indicator_active = false;
    }

    // Only run custom pipeline in custom RGB modes; let QMK handle the rest.
    uint8_t mode = rgb_matrix_get_mode();
    if (mode < RGB_MATRIX_CUSTOM) return false;

    current_custom_mode = (custom_mode_t)(mode - RGB_MATRIX_CUSTOM);

    // Periodic cache refresh (catches Vial remaps applied at runtime)
    if (timer_elapsed32(keycode_cache_timer) > KEYCODE_CACHE_REFRESH_INTERVAL) {
        update_keycode_cache();
        build_modifier_list();
        keycode_cache_timer = timer_read32();
    }

    const rgb_theme_t* theme     = get_active_theme();
    uint8_t            brightness = rgb_matrix_get_val();

    render_flag_base (led_min, led_max, theme, brightness);
    render_key_based (led_min, led_max, theme, brightness);
    render_key_groups(led_min, led_max, theme, brightness);
    render_modifier  (led_min, led_max,        brightness);
    render_os        (led_min, led_max,        brightness);

    if (current_custom_mode == CUSTOM_MODE_GAMING) {
        render_event(led_min, led_max, theme, brightness);
    }

    return false;
}