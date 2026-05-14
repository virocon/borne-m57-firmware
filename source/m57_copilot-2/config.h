#pragma once

// ============================================================
// SYSTEM / CLOCK
// ============================================================

#undef STM32_HSECLK
#define STM32_HSECLK 16000000


// ============================================================
// QMK CORE SETTINGS
// ============================================================

#define TAPPING_TOGGLE 2

// Vial dynamic keymaps
#define DYNAMIC_KEYMAP_LAYER_COUNT 10
#define DYNAMIC_KEYMAP_MACRO_COUNT 15


// ============================================================
// MATRIX CONFIGURATION
// ============================================================

#define MATRIX_ROWS 10
#define MATRIX_COLS 7

/*
Matrix wiring:
- Rows: top → bottom
- Cols: left → right
*/

#define MATRIX_ROW_PINS        { C8, C7, B2, A6, A5 }
#define MATRIX_COL_PINS        { B13, B14, B15, C6, C9, A8, C12 }

#define MATRIX_ROW_PINS_RIGHT  { A1, B7, C5, B0, B1 }
#define MATRIX_COL_PINS_RIGHT  { B8, C10, C8, C7, C6, B15, B14 }

#define DIODE_DIRECTION COL2ROW
#define DEBOUNCE 5


// ============================================================
// ENCODER
// ============================================================

#define ENCODER_MAP_KEY_DELAY 10


// ============================================================
// SPLIT COMMUNICATION (USART)
// ============================================================

#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN A9
#define SERIAL_USART_RX_PIN A10

#define SELECT_SOFT_SERIAL_SPEED 5

#define SERIAL_USART_DRIVER SD1
#define SERIAL_USART_TX_PAL_MODE 7
#define SERIAL_USART_RX_PAL_MODE 7
#define SERIAL_USART_TIMEOUT 20

// SERIAL_USART_PIN_SWAP swaps TX and RX roles in firmware (A9↔A10) without
// changing physical wiring. Normally not needed with SERIAL_USART_FULL_DUPLEX
// because both halves talk and listen simultaneously — there is no "master
// transmits first" handshake that could break.
//
// Enable ONLY if, after removing MASTER_RIGHT (dynamic USB master), plugging
// USB into the LEFT half results in garbled keypresses or the right half being
// unresponsive. That symptom means the left-as-master path is driving the wrong
// pin direction; uncommenting this line will fix it.
//#define SERIAL_USART_PIN_SWAP


// ============================================================
// SPLIT MASTER CONFIGURATION
// ============================================================

// MASTER_RIGHT is intentionally NOT defined.
// SPLIT_USB_DETECT (below) determines the master at boot:
//   - USB plugged into LEFT  → left becomes master
//   - USB plugged into RIGHT → right becomes master
// Do NOT connect USB to both halves at the same time.


// ============================================================
// SPLIT HAND DETECTION
// ============================================================

#define SPLIT_HAND_PIN C1
#define SPLIT_HAND_PIN_LOW_IS_LEFT

/*
LOW  → LEFT half
HIGH → RIGHT half
*/


// ============================================================
// SPLIT RUNTIME FEATURES
// ============================================================

#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT 2000
#define SPLIT_USB_TIMEOUT_POLL 10

#define SPLIT_MODS_ENABLE
#define SPLIT_WATCHDOG_TIMEOUT 3000
#define SPLIT_WATCHDOG_ENABLE

/*
 * USB polling interval. Lower = more responsive, but more USB interrupts → less CPU sleep.
 * Keyboard debounce (typically 5–20 ms) is the real latency floor, so sub-debounce
 * polling adds no input quality gain but keeps the CPU busier.
 *
 * Value | Rate     | USB interrupts/s | Sleep impact  | Notes
 * ------|----------|------------------|---------------|-------------------------------
 *   1   | 1000 Hz  |      1000        | minimal       | USB spec max; vendor default
 *   4   | 250 Hz   |       250        | ~4× better    | sweet spot for gaming use
 *   8   | 125 Hz   |       125        | ~8× better    | standard HID; good balance ← chosen
 *  10   | 100 Hz   |       100        | ~10× better   | QMK default
 */
#define USB_POLLING_INTERVAL_MS 8


// ============================================================
// RGB MATRIX (GROUPED SECTION)
// ============================================================

/*
IMPORTANT:

RGB matrix is enabled via rules.mk:
    RGB_MATRIX_ENABLE = yes

info.json alone is NOT sufficient.
*/

#define RGB_MATRIX_SLEEP


// ============================================================
// WS2812 (RGB DRIVER CONFIG)
// ============================================================

#define WS2812_PWM_DRIVER   PWMD3
#define WS2812_PWM_CHANNEL  2
#define WS2812_PWM_PAL_MODE 2

#define WS2812_DMA_STREAM   STM32_DMA1_STREAM2
#define WS2812_DMA_CHANNEL  5

// Optional (only needed on specific MCUs):
//#define WS2812_DMAMUX_ID STM32_DMAMUX1_TIM2_UP


// ============================================================
// EEPROM / WEAR LEVELING
// ============================================================

#define WEAR_LEVELING_LOGICAL_SIZE 4096
#define WEAR_LEVELING_BACKING_SIZE (WEAR_LEVELING_LOGICAL_SIZE * 2)

#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 4095


// ============================================================
// FINAL FLAGS
// ============================================================

#define NEW_401_BL


// ============================================================
// LEGACY / UNUSED (COMMENTED OUT FOR REFERENCE)
// ============================================================

/*
//#define FORCE_NKRO
//#define LOCKING_SUPPORT_ENABLE
//#define LOCKING_RESYNC_ENABLE
//#define TAPPING_TERM 100
//#define TAP_CODE_DELAY 15
//#define QMK_KEYS_PER_SCAN 4
//#define EE_HANDS
*/