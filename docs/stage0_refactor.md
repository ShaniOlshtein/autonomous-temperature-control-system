# Stage 0 — split the existing sketch into modules (NO behavior change)

Source: `cooking_temperature_control.ino` (382 lines, commit 3e7faf9). Branch: `refactor/stage0-modules`.
Goal: separate files with one owner each, so later stages do not collide. After this stage the device must look and behave exactly as before.

## Hard rules
- Move code; do not rewrite logic. Same pins, same timings, same screen, same button behavior.
- Keep today's quirks and report them for Stage 1: `timerRunning = true` at boot, `isHot` is cosmetic, temperatures are shown as whole numbers (`(int)temp`), `targetTemp` below 0 becomes 0.
- No relay, no BLE, no NVS, no new libraries. The timer keeps today's single `remainingSeconds` value (the duration/remaining split comes in Stage 2).
- All new `.h/.cpp` files live in the sketch folder next to the `.ino`. Use `#pragma once`. Pins come from `pins.h`.

## Mapping (PROPOSED — owners finalize in preflight)
| Today in the `.ino` | Goes to | Owner |
|---|---|---|
| `LCD_BL`, `ADKEY_OUT_PIN`, `TEMP_SENSOR_PIN`, `TEMP_SENSOR_COUNT` | `pins.h` | hardware-pins |
| screen constants, `tft`, `sprTimer`/`sprTarget`/`sprCurrent`, `renderTimer`, `renderTargetTemp`, `renderCurrentTemp`, `drawStaticUI`, `updateSelectionBlink`, `selectedField`, `selectedFieldVisible` | `display_ui.*` | display-ui |
| `readAdKey`, `updateButtons`, `updateLongPress`, `handleActionButton`, `moveSelection` | `input.*` | display-ui |
| `changeSelectedValue` | `input.*`, calling setters instead of editing globals | display-ui |
| `targetTemp`, `isHot` (with the existing clamp) | `config_store.*` (thin holder) | config-store |
| `remainingSeconds`, `timerRunning`, the countdown in `loop()` | `timer.*` | control-logic |
| `updateTemperature`, OneWire, DallasTemperature, `currentTemp`, conversion timing | `sensors.*` | control-logic |
| `setup()`, `loop()` | main `.ino`: only init/update calls | control-logic |

Timing variables and constants move together with the code that uses them (so nothing stays orphaned in the `.ino`):
- `timer.*`: `previousMillis` (the one-second countdown timestamp).
- `sensors.*`: `temperatureRequestMillis`, `temperatureConversionPending`.
- `display_ui.*`: screen constants (`SCREEN_WIDTH`, `SCREEN_HEIGHT`, `COL_WIDTH`, `HEADER_HEIGHT`, `CONTENT_START_X`, `CONTENT_START_Y`, `SPR_WIDTH`, `SPR_HEIGHT`), `lastBlinkMillis`, `SELECTION_BLINK_MS`.
- `input.*`: `lastAdKeyDebounceMillis`, `lastAdKeyReading`, `adKeyState`, `ADKEY_DEBOUNCE_MS`, `LONG_PRESS_MS`, `LONG_PRESS_REPEAT_MS`, `actionPressMillis`, `lastLongActionMillis`, `longActionStarted`.

## Order of work
1. Preflight with every owner (READY/BLOCKED).
2. **Contracts first:** each owner writes only its `.h` file (function names and types). The Lead checks for overlaps and gaps, and the human approves.
3. Implement the `.cpp` files and the slim `.ino`.
4. `qa-safety` compares old and new code by reading and lists every difference.
5. The human compiles, uploads and runs the checklist below.
6. Commit only after the human confirms.

## Acceptance checklist (the human runs it on the device, before and after)
- Boot: same grid, headers "Timer", "Target Temp", "Current Temp".
- Timer column: ON in green with the countdown; OFF in red; counts down once per second.
- Target column: whole number followed by "C"; HOT in red or COLD in cyan.
- Current column: average of the valid sensors as a whole number, or `ERR` with no valid sensor. One sensor unplugged: still shows a value. Both unplugged: `ERR`.
- The selected field blinks (about 0.4 s).
- SW4 moves the selection forward (ON/OFF, timer value, target, HOT/COLD, back to start); SW1 moves backward, with wrap-around.
- SW2 decreases and SW3 increases: timer short press 1 s, hold 1 min per step; target short press 1 degree, hold 5 degrees per step; ON/OFF and HOT/COLD toggle with either button.
- Nothing else changed: no flicker, no new messages.

## Arduino watch-outs
- Globals shared between files need `extern` or accessor functions; prefer accessors.
- Do not rename the main `.ino`; the sketch folder name must match it.
- Only one `TFT_eSPI` instance; `display_ui.*` owns it and the sprites.
- The TFT_eSPI setup selection (Setup206) is library-level and must not be touched.
