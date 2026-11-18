# Stage 0 — split the existing sketch into modules (NO behavior change)

Source: `cooking_temperature_control.ino` (479 lines, on `main` at commit 43e3339, includes the SW5 START/STOP work). Branch: `refactor/stage0-modules` from `main`.
Goal: separate files with one owner each, so later stages do not collide. After this stage the device must look, print and behave exactly as before.

## Hard rules
- Move code; do not rewrite logic. Same pins, same timings, same screen, same button behavior, same Serial output, same order of calls in `setup()` and `loop()`.
- Keep every current quirk unchanged (decision 15). They are listed in `docs/open_questions.md` section E for Stage 1. Do not fix them now.
- No relay, no BLE, no NVS, no new libraries. The timer keeps today's single `remainingSeconds` value (the duration/remaining split comes in Stage 2).
- All new `.h/.cpp` files live in the sketch folder next to the `.ino`. Use `#pragma once`. Pins come from `pins.h`; the `#define`s for `LCD_BL`, `ADKEY_OUT_PIN`, `TEMP_SENSOR_PIN`, `TEMP_SENSOR_COUNT` are removed from the `.ino` (same values already in `pins.h`).

## Mapping — every symbol in the original `.ino` (confirmed in preflight; final function names in "Approved contracts" below)

### Includes and objects
| Today in the `.ino` | Goes to | Owner |
|---|---|---|
| `#include <SPI.h>`, `<TFT_eSPI.h>`, `tft`, `sprTimer`, `sprTarget`, `sprCurrent` | `display_ui.*` | display-ui |
| `#include <OneWire.h>`, `<DallasTemperature.h>`, `oneWire`, `temperatureSensor` | `sensors.*` | control-logic |
| `LCD_BL`, `ADKEY_OUT_PIN`, `TEMP_SENSOR_PIN`, `TEMP_SENSOR_COUNT` | `pins.h` (already there, no change expected) | hardware-pins |

### Constants
| Symbol | Goes to | Owner |
|---|---|---|
| `SCREEN_WIDTH`, `SCREEN_HEIGHT`, `COL_WIDTH`, `HEADER_HEIGHT`, `CONTENT_START_X`, `CONTENT_START_Y`, `SPR_WIDTH`, `SPR_HEIGHT`, `SELECTION_BLINK_MS` | `display_ui.*` | display-ui |
| `ADKEY_DEBOUNCE_MS`, `LONG_PRESS_MS`, `LONG_PRESS_REPEAT_MS`, `SW5_STOP_PRESS_MS` | `input.*` | display-ui |
| magic numbers 750 ms / 2000 ms in `updateTemperature` | stay inline in `sensors.cpp` (no renaming) | control-logic |
| magic number 1000 ms in the countdown | stays inline in `timer.cpp` | control-logic |

### Variables
| Symbol | Goes to | Owner |
|---|---|---|
| `targetTemp`, `isHot` | `config_store.*` (thin holder with the existing clamp `< 0 => 0`) | config-store |
| `remainingSeconds`, `timerEnabled`, `cookingActive`, `previousMillis` | `timer.*` | control-logic |
| `currentTemp`, `temperatureRequestMillis`, `temperatureConversionPending` | `sensors.*` | control-logic |
| `uiBackgroundColor`, `uiNeedsRedraw`, `SelectionField` enum, `selectedField`, `selectedFieldVisible`, `lastBlinkMillis` | `display_ui.*` | display-ui |
| `lastAdKeyDebounceMillis`, `lastAdKeyReading`, `adKeyState`, `lastAdcValue`, `lastAdcDebugMillis`, `actionPressMillis`, `lastLongActionMillis`, `longActionStarted`, `sw5PressMillis`, `sw5LongPressHandled` | `input.*` | display-ui |

### Functions
| Symbol | Goes to | Owner |
|---|---|---|
| `renderTimer`, `renderTargetTemp`, `renderCurrentTemp`, `drawStaticUI`, `updateSelectionBlink`, `updateCookingDisplayColor` | `display_ui.*` | display-ui |
| the `mm:ss` string built in `loop()` | `display_ui.*` (render receives the seconds) | display-ui |
| the `if (uiNeedsRedraw) { drawStaticUI(); ... }` block in `loop()` | `display_ui.*` | display-ui |
| `readAdKey` (with the ADC thresholds), `updateButtons` (with the 250 ms ADC debug print), `updateLongPress`, `handleActionButton`, `handleSw5Press`, `handleSw5LongPress`, `moveSelection` | `input.*` | display-ui |
| `changeSelectedValue` | `input.*`, calling setters of `timer.*` and `config_store.*` instead of editing globals | display-ui |
| `setCookingActive`, `initializeTimerForCooking` | `timer.*` (see the coupling rule below) | control-logic |
| `updateTemperature` and the sensor block of `setup()` (begin, device-count print, `setWaitForConversion(false)`, first request) | `sensors.*` | control-logic |
| the countdown block of `loop()` | `timer.*` | control-logic |
| ADC pin setup in `setup()` (`pinMode`, `analogSetPinAttenuation`) | `input.*` init | display-ui |
| backlight, `tft.init`, rotation, `fillScreen`, `createSprite`, first `drawStaticUI` | `display_ui.*` init | display-ui |
| `setup()`, `loop()`, `Serial.begin` | main `.ino`: only init/update calls | control-logic |

### Coupling rule: `cookingActive` and `uiNeedsRedraw`
Today `setCookingActive()` sets `cookingActive` and then calls `updateCookingDisplayColor()`, which sets `uiBackgroundColor` and `uiNeedsRedraw = true` **on every call**, even when the value did not change. The redraw runs later in the same `loop()` pass.
- **Allowed split (no timing change):** `input.*` keeps the exact call order: set cooking state in `timer.*`, then call the display color/redraw function in `display_ui.*`, then (START only) `initializeTimerForCooking`.
- **Not allowed:** the display polls `cookingActive` and redraws only when it changes. That drops the redraw on a repeated SW5 press (a behavior change).
- If an owner finds the allowed split impossible, do NOT split: keep `cookingActive`, `setCookingActive`, `updateCookingDisplayColor`, `uiBackgroundColor` and `uiNeedsRedraw` together in one module and report it to the Lead.

### Call order that must be preserved
- `setup()`: Serial → backlight ON → ADC pin setup → sensors init → TFT init, sprites, `drawStaticUI`.
  Backlight currently comes before sensors init; if `display_ui` init cannot be split, report it (moving the backlight later is a timing change).
- `loop()`: buttons → selection blink → temperature → static redraw if needed → countdown → render timer, target, current.

## Approved contracts (decision 19) — the real interface
Rule: every public function has a module prefix; everything else is `static` in its `.cpp`. Old names in brackets.

| Header | Owner | Public functions |
|---|---|---|
| `config_store.h` | config-store | `float configGetTargetTemp()`, `void configSetTargetTemp(float)` (stores, then `< 0 => 0`), `bool configGetIsHot()`, `void configSetIsHot(bool)`. Defaults 65.0 / true in `config_store.cpp`; no init function |
| `sensors.h` | control-logic | `void sensorsInit()` (sensor block of old `setup()`), `void sensorsUpdate()` [`updateTemperature`], `float sensorsGetCurrentTemp()` |
| `timer.h` | control-logic | `void timerUpdate()` (countdown block of old `loop()`), `bool timerIsEnabled()`, `void timerToggleEnabled()`, `int timerGetRemainingSeconds()`, `void timerAddSeconds(int)` (adds, then `< 0 => 0`; the only place of this clamp), `bool timerIsCookingActive()`, `void timerSetCookingActive(bool)` [state part of `setCookingActive`; never calls display], `void timerInitializeForCooking()` [`initializeTimerForCooking`] |
| `display_ui.h` | display-ui | `enum SelectionField`, `void displayUiBacklightOn()`, `void displayUiInit()`, `void displayUiUpdateBlink()` [`updateSelectionBlink`], `void displayUiRedrawIfNeeded()`, `void displayUiRender()` (builds `mm:ss`, renders the 3 columns, reads the getters), `void displayUiSetCookingColor(bool)` [`updateCookingDisplayColor`; requests a redraw on every call], `SelectionField displayUiGetSelectedField()`, `void displayUiSelectField(SelectionField)` |
| `input.h` | display-ui | `void inputInit()` (ADC pin setup), `void inputUpdate()` [`updateButtons`] |
| `pins.h` | hardware-pins | unchanged |

Coupling rule as implemented: SW5 START = print → `timerSetCookingActive(true)` → `displayUiSetCookingColor(true)` → `timerInitializeForCooking()`; STOP = print → `timerSetCookingActive(false)` → `displayUiSetCookingColor(false)`. Same order as the old `setCookingActive` + `initializeTimerForCooking`, so no redraw timing change.

Slim `.ino`:
- `setup()`: `Serial.begin(115200)` → `displayUiBacklightOn()` → `inputInit()` → `sensorsInit()` → `displayUiInit()`
- `loop()`: `inputUpdate()` → `displayUiUpdateBlink()` → `sensorsUpdate()` → `displayUiRedrawIfNeeded()` → `timerUpdate()` → `displayUiRender()`

## Order of work
1. Preflight with every owner (READY/BLOCKED).
2. **Contracts first:** each owner writes only its `.h` file (function names and types). The Lead checks for overlaps and gaps, and the human approves.
3. Implement the `.cpp` files and the slim `.ino`, in dependency order: `config_store` → `sensors`, `timer` → `display_ui`, `input` → `.ino`.
4. `qa-safety` compares old and new code by reading and lists every difference (logic, timing, call order, Serial text).
5. The human compiles, uploads and runs the checklist below.
6. Commit only after the human confirms.

## Acceptance checklist (the human runs it on the device, before and after)
- Boot: same grid, headers "Timer", "Target Temp", "Current Temp"; black background; timer OFF in red.
- Timer column: ON in green with the `mm:ss` value; OFF in red with no value.
- Target column: whole number followed by "C"; HOT in red or COLD in cyan.
- Current column: average of the valid sensors as a whole number, or `ERR` with no valid sensor. One sensor unplugged: still shows a value. Both unplugged: `ERR`.
- The selected field blinks (about 0.4 s).
- SW4 moves the selection forward (ON/OFF, timer value, target, HOT/COLD, back to start); SW1 moves backward, with wrap-around.
- SW2 decreases and SW3 increases: timer short press 1 s, hold 1 min per step; target short press 1 degree, hold 5 degrees per step; ON/OFF and HOT/COLD toggle with either button.
- SW5 short press: background turns dark green (START); Serial prints `SW5 press: START`.
- SW5 held 1.2 s: background back to black (STOP); Serial prints `SW5 press: START` then `SW5 long press: STOP`.
- Countdown: decreases once per second only when the background is green AND the timer is ON; stops at 00:00.
- Serial: `ADC=<value> decoded=SW<n>` about every 250 ms; `Button state: SW<n>` on each change; per-sensor lines and `Valid temperature sensors: n/2` about every 2 s; `OneWire devices found: n` at boot.
- Nothing else changed: no new flicker, no new messages.

## Arduino watch-outs
- Globals shared between files need `extern` or accessor functions; prefer accessors.
- Do not rename the main `.ino`; the sketch folder name must match it.
- Only one `TFT_eSPI` instance; `display_ui.*` owns it and the sprites.
- The TFT_eSPI setup selection (Setup206) is library-level and must not be touched.
- `SelectionField` is needed by both `display_ui` and `input` (same owner): declare it once in `display_ui.h`.
