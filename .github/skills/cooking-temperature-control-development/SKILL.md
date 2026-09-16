---
name: cooking-temperature-control-development
description: 'Develop and verify the Cooking Temperature Control Arduino project. Use for ESP32-S3 LilyGo T-Display changes involving TFT_eSPI, the 320x170 UI, timer behavior, buttons, GPIO, temperature sensors, documentation, compilation, or upload.'
argument-hint: 'Describe the feature, bug, hardware change, or verification task.'
user-invocable: true
---

# Cooking Temperature Control Development

## Outcome

Apply a focused, hardware-aware change to the Cooking Temperature Control project and leave the repository documented and validated. Keep the project easy to extend from its current UI prototype toward sensor-driven temperature control.

## Use When

- Adding or changing a display view, value, sprite, or layout.
- Changing countdown, timing, or control-state behavior.
- Adding button interaction or a temperature sensor.
- Changing GPIO, display wiring, or TFT_eSPI configuration.
- Updating project documentation or preparing a compile/upload check.

## Procedure

1. Read `AGENTS.md`, `README.md`, `cooking_temperature_control.ino`, and the relevant TFT_eSPI setup before editing.
2. Identify the change category: input handling, state management, sensor acquisition, timing, rendering, hardware configuration, or documentation.
3. Check hardware dependencies before changing code:
   - Use `libraries/arduino_853159/User_Setup_Select.h` to identify the selected setup.
   - Use `libraries/arduino_853159/User_Setups/Setup206_LilyGo_T_Display_S3.h` as the display wiring and driver source of truth.
   - Preserve `LCD_BL=38`, `ADKEY_OUT_PIN=2`, and `TEMP_SENSOR_PIN=3` unless the hardware change is intentional and documented.
4. State the expected behavior and hardware impact, then make the smallest root-cause edit.
5. Keep responsibilities separate:
   - Read buttons and sensors in input/acquisition code.
   - Update `remainingSeconds`, `timerRunning`, `targetTemp`, and `currentTemp` in state/control code.
   - Keep drawing in render functions and preserve the ownership of `sprTimer`, `sprTarget`, and `sprCurrent`.
   - Use `millis()` for recurring timing; avoid blocking delays.
6. Update `README.md` when behavior, wiring, sensors, inputs, or setup changes. Document sensor wiring and configuration explicitly.
7. Validate the result:
   - Review the changed code for pin conflicts, stale UI values, unsafe assumptions, and accidental TFT_eSPI changes.
   - Compile with Arduino CLI when the board package and exact FQBN are available:

   ```bash
   arduino-cli compile --fqbn <ESP32S3_FQBN> ./
   ```

   - Upload only after an explicit user request and confirmation of the target port:

   ```bash
   arduino-cli upload -p <PORT> --fqbn <ESP32S3_FQBN> ./
   ```

## Decision Rules

- If a request changes display pins or driver settings, inspect both TFT_eSPI setup files before editing and explain the hardware impact.
- If a request adds a sensor, define its wiring and reading schedule before connecting it to `currentTemp`.
- If a request adds button behavior, account for `INPUT_PULLUP`, active-low presses, and debouncing without blocking the main loop.
- If a request changes the layout, preserve the landscape `320x170` geometry and update sprite dimensions and static divider drawing together.
- If the FQBN, board package, or port is unknown, do not guess; report what is missing and stop before upload.

## Completion Checklist

- [ ] The relevant project and hardware files were read.
- [ ] The change category and hardware impact were identified.
- [ ] No display or GPIO mapping was changed accidentally.
- [ ] Timing remains non-blocking.
- [ ] State updates and rendering remain separated.
- [ ] `README.md` was updated when the user-facing behavior or wiring changed.
- [ ] A focused compile or other available validation was run.
- [ ] Upload was performed only with explicit approval and a confirmed port.
