# m57_copilot-2 vs m57_claude — Source Code Comparison

Generated: 2026-05-17

---

## 1. Programming Paradigm

| | copilot-2 | claude |
|---|---|---|
| Style | Procedural C with light OO-ish naming | Procedural C with stricter data encapsulation |
| State management | Mix of `static` globals and `current_custom_mode` global variable | All mutable state either in `user_config` (EEPROM) or tightly-scoped `static` locals |
| Side effects | `current_custom_mode` mutated inside `rgb_matrix_indicators_advanced_user` — shared side-effecting state | `active_ui_mode` is a local variable computed fresh each frame — no shared mutation |

**Winner: claude** — stricter avoidance of shared mutable state.

---

## 2. Coding Conventions

| | copilot-2 | claude |
|---|---|---|
| Comment style | `// C++ style` throughout | `/* C style */` throughout — correct for C99 embedded |
| Section headers | `// ===` (C++ comment) | `/* === */` (C comment) |
| Naming: custom keycodes | `OS_LINUX_SET`, `OS_WINDOWS_SET` | `OS_SET_LINUX`, `OS_SET_WINDOWS` — verb-first, consistent |
| Naming: render functions | `render_flag_base`, `render_key_based`, `render_key_groups`, `render_modifier`, `render_os`, `render_layer`, `render_lock_states`, `render_event` | `render_background`, `render_action_keys`, `render_key_classes`, `render_mod_keys`, `render_os_indicator`, `render_layer_indicator`, `render_lock_indicators`, `render_reactive` |
| Naming: theme fields | `layer1`, `layer2` — abstract, unclear | `accent`, `body` — semantic, self-documenting |
| Naming: enum values | `CUSTOM_MODE_UI/DEBUG/GAMING` | `UI_MODE_DEFAULT/DEBUG/GAMING` |
| Log format | `[TAG] key=value` — structured prefix | `key=value` — no prefix, slightly less scannable |
| Trailing commas in enums | No | Yes — correct C99 |

**Winner: claude** — C-style comments, semantic naming, verb-first keycode names. copilot-2's `layer1`/`layer2` field names are particularly opaque.

---

## 3. Design Patterns

| | copilot-2 | claude |
|---|---|---|
| Color system | Two-level: `COLOR_BASE_UI` + `COLOR_BASE` — theme colors defined as named intermediates, then referenced in structs | Single-level: `RGB_COLOR(r,g,b)` macro + direct struct init — fewer indirections |
| Theme lookup | `themes[]` array, direct index | `theme_table[]` array with designated initializers `[UI_MODE_X] = &theme_x` |
| Unicode sync | Inline `switch` in 3 separate places — code duplicated | Extracted `sync_unicode_mode()` helper — single source of truth |
| Config load/save | `load_config_from_eeprom()` / `save_config()` | `load_config()` / `save_config()` — simpler naming |
| Boot animation | Single inline implementation | Refactored to 3 named functions + random selection |
| Caps Word | Not present | ✅ `CAPS_WORD_ENABLE` + double-tap Shift |
| Debug logging | Compile-time only | ✅ Runtime toggle + level, persisted to EEPROM |
| Shift+Backspace→Delete | Not present | ✅ Key override |

**Winner: claude** — `sync_unicode_mode()` abstraction alone saves 3 copies of identical switch logic. Also has more features.

---

## 4. Architectural Style

| | copilot-2 | claude |
|---|---|---|
| Mode vs theme decoupling | `current_custom_mode` is a separate concept from `user_config.theme` — mode = behavior, theme = colors. Explicitly documented in a comment. | `ui_mode_t` serves both roles — same value is both RGB mode offset and theme index |
| Separation of concerns | theme.h richer: has `COLOR_BASE_*` intermediate constants for each theme, separate from global constants | theme.h leaner: direct `RGB_COLOR()` in struct initializers |
| Correctness of decoupling | copilot-2's mode/theme split is architecturally sounder if you ever want "gaming behavior with UI colors" | claude's dual-use is simpler but less flexible |
| `rules.mk` documentation | Very detailed — each flag has a multi-line explanation with examples | Compact — one-liner per flag |

**Winner: split.** copilot-2 has a genuinely better architecture for mode/theme separation. claude's `rules.mk` is cleaner but copilot-2's is more educational.

---

## 5. Vial-QMK / QMK Compatibility

| | copilot-2 | claude |
|---|---|---|
| Redundant includes | `eeconfig.h`, `print.h`, `host.h` — already in `QMK_KEYBOARD_H` chain | ✅ Only `QMK_KEYBOARD_H` + `theme.h` + `stdlib.h` |
| `rgb_matrix_mode()` in layer hook | ❌ Uses `rgb_matrix_mode()` — **writes to EEPROM on every GAME layer toggle** | ✅ Uses `rgb_matrix_mode_noeeprom()` — no spurious EEPROM writes |
| `combo_t key_combos[] = {}` | ❌ GNU zero-init extension — not valid strict C99 | ✅ `combo_t key_combos[COMBO_COUNT]` — standard |
| `combo_placeholder[]` | ❌ Unused array, dead code | ✅ Removed |
| KC_1..KC_0 digit range | `kc >= KC_1 && kc <= KC_9 \|\| kc == KC_0` — two separate checks | ✅ `kc >= KC_1 && kc <= KC_0` — single range (leverages QMK keycode ordering) |
| `CAPS_WORD_ENABLE` | ❌ Not present | ✅ Present |
| Debug logging | Compile-time only | ✅ Runtime toggle, EEPROM persisted |

**Winner: claude** — the `rgb_matrix_mode()` vs `rgb_matrix_mode_noeeprom()` bug alone is a significant correctness issue in copilot-2.

---

## 6. Better Implementation Per Feature

| Feature | Better in | Reason |
|---|---|---|
| Unicode sync | claude | Extracted helper, not duplicated 3× |
| Theme field naming | claude | `accent`/`body` vs opaque `layer1`/`layer2` |
| Mode/theme architecture | copilot-2 | Explicit decoupling is sounder long-term |
| rules.mk documentation | copilot-2 | Much more educational for new contributors |
| config.h documentation | copilot-2 | Inline explanations per setting are better |
| Log format | copilot-2 | `[TAG]` prefix makes logs scannable |
| Debug system | claude | Runtime toggle + EEPROM persistence |
| Boot animation | claude | 3 styles, random selection |
| Digit keycode range | claude | Single elegant range check |
| GAME layer RGB switch | claude | `noeeprom` variant — no EEPROM wear |
| Caps Word | claude | Not in copilot-2 at all |

---

## 7. Additional Criteria

### Dead code

- copilot-2: `combo_placeholder[]` unused array; 3 redundant includes (`eeconfig.h`, `print.h`, `host.h`)

### EEPROM wear (correctness bug)

copilot-2 calls `rgb_matrix_mode()` inside `layer_state_set_user`. That function writes to EEPROM on every call. Every GAME layer toggle (FN+V) triggers an EEPROM write. Over thousands of toggles this accelerates flash wear on the STM32F401. claude uses `rgb_matrix_mode_noeeprom()` — no wear.

### Operator precedence risk (digit range check)

copilot-2:
```c
(kc >= KC_1 && kc <= KC_9) || kc == KC_0
```
Works by coincidence of C operator precedence (`&&` binds tighter than `||`), but is missing explicit parentheses around the `|| kc == KC_0` arm — a maintenance hazard. claude uses a single contiguous range:
```c
kc >= KC_1 && kc <= KC_0
```
which is cleaner and relies on QMK's guaranteed keycode ordering.

---

## Overall Verdict

| Project | Strength |
|---|---|
| **copilot-2** | Better documentation in rules.mk/config.h; better mode/theme architectural separation; structured `[TAG]` log prefixes |
| **claude** | Fewer bugs; more features; better naming; cleaner C conventions; runtime debug; EEPROM-safe layer switching |

**Recommendation:** `m57_claude` is the better production base. Two things worth porting from copilot-2:

1. **rules.mk / config.h documentation style** — copilot-2's multi-line explanations per flag are more educational.
2. **`[TAG]` log prefix format** — `[INIT]`, `[KEY]`, `[ENC]` prefixes make console output scannable at a glance.
