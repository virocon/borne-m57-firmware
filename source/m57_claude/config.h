

#pragma once

/* ============================================================
 * KEYBOARD: borne m57  (crkbd/corne variant, split, STM32F401)
 * ============================================================
 * This is the top-level hardware configuration for the m57 keyboard definition.
 * It applies to both halves — hand detection at boot decides master vs slave role.
 * Keymap-specific settings (tapping term, mouse speed, Vial slots) are in
 * keymaps/via/config.h, which overrides or extends these values. */


/* ============================================================
 * CLOCK
 * ============================================================
 * The m57 uses a 16 MHz external crystal (HSE).  Override the QMK default of
 * 8 MHz so the PLL in mcuconf.h can target 48 MHz for USB. */
#undef STM32_HSECLK
#define STM32_HSECLK 16000000


/* ============================================================
 * LAYER / MACRO COUNTS
 * ============================================================ */

/* How many times a key must be tapped to toggle a TG() layer on/off.
 * 2 = double-tap toggles.  Used with TG(LAYER_GAME) on FN+V. */
#define TAPPING_TOGGLE 2

/* Number of layers VIA/Vial exposes for live remapping.
 * 10 = layers 0-9; layers 4-9 are fully user-programmable at runtime.
 * Increasing this costs EEPROM — do not exceed DYNAMIC_KEYMAP_EEPROM_MAX_ADDR / layer size. */
#define DYNAMIC_KEYMAP_LAYER_COUNT 10

/* Number of macro slots available in the Vial macro editor (stored in EEPROM). */
#define DYNAMIC_KEYMAP_MACRO_COUNT 15


/* ============================================================
 * MATRIX DIMENSIONS
 * ============================================================
 * The m57 is a 5-row × 7-col split keyboard, but QMK sees it as a single
 * 10-row × 7-col unified matrix:
 *   Rows 0-4  = left half
 *   Rows 5-9  = right half
 * The LAYOUT macro in m57.h maps physical key positions to this matrix. */
#define MATRIX_ROWS 10
#define MATRIX_COLS 7

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
 */

/* Left half row/col pins (STM32F401 GPIO).
 * Right half uses separate pin sets because the MCU pads differ between PCBs. */
#define MATRIX_ROW_PINS         { C8, C7, B2, A6, A5 }
#define MATRIX_COL_PINS         { B13, B14, B15, C6, C9, A8, C12 }
#define MATRIX_ROW_PINS_RIGHT   { A1, B7, C5, B0, B1 }
#define MATRIX_COL_PINS_RIGHT   { B8, C10, C8, C7, C6, B15, B14 }

/* Alternative pin assignment (swap left/right if board is physically mirrored):
//#define MATRIX_ROW_PINS       { A1, B7, C5, B0, B1 }
//#define MATRIX_COL_PINS       { B8, C9, C8, C7, C6, B15, B14 }
//#define MATRIX_ROW_PINS_RIGHT { C8, C7, B2, A6, A5 }
//#define MATRIX_COL_PINS_RIGHT { B13, B14, B15, C6, C9, A8, C12 } */

/* Diodes run column-to-row (standard for most split keyboards). */
#define DIODE_DIRECTION COL2ROW

/* Debounce time in ms.  5 ms is the QMK default and works well for most switches. */
#define DEBOUNCE 5

//#define FORCE_NKRO         // Uncomment to force N-key rollover; requires USB HID boot protocol to be disabled.


/* ============================================================
 * SPLIT HALF TRANSPORT  (full-duplex USART1 on A9/A10)
 * ============================================================
 * Both halves run identical firmware; whichever half has USB plugged in becomes
 * master.  The slave half communicates over USART1 at 19200 baud.
 *
 * Full-duplex mode uses separate TX and RX lines — more reliable than
 * single-wire half-duplex at the cost of one extra trace. */
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN     A9
#define SERIAL_USART_RX_PIN     A10
//#define USART1_REMAP           // Remap USART pins (STM32F103 only — not needed on F401).
//#define SERIAL_USART_PIN_SWAP  // Swap TX/RX if the wired-up half reports as inverted.

/* Baud rate selection.  Speed 5 = 19200 baud — conservative choice for reliability.
 * Higher speeds reduce latency but are more sensitive to cable length/quality.
 *   0: 460800 baud
 *   1: 230400 baud  (QMK default)
 *   2: 115200 baud
 *   3:  57600 baud
 *   4:  38400 baud
 *   5:  19200 baud  ← current */
#define SELECT_SOFT_SERIAL_SPEED 5

#define SERIAL_USART_DRIVER         SD1  /* USART peripheral (USART1 → SD1 in ChibiOS). */
#define SERIAL_USART_TX_PAL_MODE    7    /* Alternate function 7 = USART1 on STM32F401. */
#define SERIAL_USART_RX_PAL_MODE    7
#define SERIAL_USART_TIMEOUT        20   /* ms before the transport layer gives up on a frame. */


/* ============================================================
 * SPLIT HAND DETECTION
 * ============================================================
 * MASTER_RIGHT: the right half is always the USB master.  The slave (left)
 *   connects via USART1 and proxies its matrix through the master.
 *
 * SPLIT_HAND_PIN C1: a GPIO read at boot distinguishes left from right.
 * SPLIT_HAND_PIN_LOW_IS_LEFT: pin pulled low on the left PCB, high on the right.
 *   This means both halves run the same binary — no per-half flash needed. */
#define MASTER_RIGHT
#define SPLIT_HAND_PIN              C1
#define SPLIT_HAND_PIN_LOW_IS_LEFT
//#define SPLIT_LAYER_STATE_ENABLE   // Sync active layer to slave (costs bandwidth).
//#define SPLIT_LED_STATE_ENABLE     // Sync LED state to slave.


/* ============================================================
 * SPLIT USB / WATCHDOG
 * ============================================================
 * SPLIT_USB_DETECT: poll for USB VBUS instead of relying on hardware detection.
 *   Required on STM32F401 because there is no dedicated VBUS sense pin.
 * SPLIT_WATCHDOG_ENABLE: reset the slave if it stops responding within TIMEOUT ms.
 *   Prevents the keyboard from going silent after a transport glitch. */
#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT           2000  /* ms to wait for USB enumeration before assuming slave role. */
#define SPLIT_USB_TIMEOUT_POLL      10    /* ms between USB detect polls during the timeout window. */
#define SPLIT_MODS_ENABLE                 /* Sync modifier state to slave for correct LED rendering. */
#define SPLIT_WATCHDOG_ENABLE
#define SPLIT_WATCHDOG_TIMEOUT      3000  /* ms before slave watchdog fires a reset. */
#define USB_POLLING_INTERVAL_MS     1     /* USB HID report interval; 1 ms = fastest (125 Hz effective). */


/* ============================================================
 * RGB MATRIX  (WS2812 driven by TIM3 CH2 via DMA, pin A7)
 * ============================================================
 * 58 LEDs total: 29 per half.  PWM output on A7, alternate function 2 (TIM3_CH2).
 * DMA stream 2 channel 5 handles the bit-banging so the CPU is not blocked.
 * RGB_MATRIX_SLEEP: dim LEDs after idle timeout to reduce power draw. */
#define WS2812_PWM_DRIVER       PWMD3            /* TIM3 peripheral. */
#define WS2812_PWM_CHANNEL      2                /* TIM3 channel 2 → PA7. */
#define WS2812_PWM_PAL_MODE     2                /* Alternate function 2 = TIM3_CH2 on STM32F401. */
#define WS2812_DMA_STREAM       STM32_DMA1_STREAM2  /* DMA1 stream 2 = TIM3_UP on F401. */
#define WS2812_DMA_CHANNEL      5                /* DMA channel 5 for TIM3_UP. */
//#define WS2812_DMAMUX_ID      STM32_DMAMUX1_TIM2_UP  // Only needed on MCUs with DMAMUX (e.g. G4, L4+).
#define RGB_MATRIX_SLEEP


/* ============================================================
 * EEPROM / WEAR LEVELING
 * ============================================================
 * QMK uses wear-leveling over internal flash to store keymaps, macros,
 * and user config (OS, theme, debug state).
 *
 * LOGICAL_SIZE  4096 bytes  — address space visible to QMK.
 * BACKING_SIZE  8192 bytes  — actual flash consumed (2× logical for leveling).
 * EEPROM_MAX_ADDR 4095      — must equal LOGICAL_SIZE - 1; raising it past 4095
 *                             without growing LOGICAL_SIZE will corrupt data. */
#define WEAR_LEVELING_LOGICAL_SIZE      4096
#define WEAR_LEVELING_BACKING_SIZE      (WEAR_LEVELING_LOGICAL_SIZE * 2)
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR  4095


/* ============================================================
 * BOOTLOADER
 * ============================================================
 * PlumBL soft-entry magic for the STM32F401 variant.
 * The actual jump is implemented in m57.c (bootloader_jump override). */
#define NEW_401_BL


/* ============================================================
 * ENCODER
 * ============================================================ */
/* Delay (ms) injected between the key-down and key-up events synthesized for
 * each encoder click.  10 ms is safe for all OS key-repeat debounce thresholds. */
#define ENCODER_MAP_KEY_DELAY 10


/* ============================================================
 * UNUSED / REFERENCE  (commented-out vendor options)
 * ============================================================
 * These were present in the vendor source and are kept for reference.
 * Uncomment only if the corresponding feature is enabled in rules.mk. */
//#define LOCKING_SUPPORT_ENABLE
//#define LOCKING_RESYNC_ENABLE
//#define TAP_CODE_DELAY 15
//#define QMK_KEYS_PER_SCAN 4
//#define EE_HANDS               // Alternative hand detection via EEPROM flag (vs pin).
//#define VIA_HAS_BROKEN_KEYCODES