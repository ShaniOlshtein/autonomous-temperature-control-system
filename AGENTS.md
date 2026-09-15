# AGENTS.md

## Project: Cooking Temperature Control

This workspace contains an Arduino project for a cooking temperature control UI running on an ESP32-S3 LilyGo T-Display board.

## Hardware configuration

The display setup is already configured for the LilyGo T-Display S3 board:

- `TFT_eSPI` user setup selected in `User_Setup_Select.h`
- Active setup: `Setup206_LilyGo_T_Display_S3.h`
- Display type: ST7789, 170x320 TFT
- Board family: ESP32-S3

This configuration should be treated as the source of truth for the display wiring and driver setup.

## Relevant project files

- `cooking_temperature_control.ino` — main sketch
- `README.md` — project overview and usage notes
- `.agent.md` — developer-specific agent notes
- `libraries/arduino_853159/User_Setup_Select.h` — selected TFT_eSPI setup file
- `libraries/arduino_853159/User_Setups/Setup206_LilyGo_T_Display_S3.h` — board-specific display configuration

## Current GPIO usage in the sketch

The sketch currently defines these pins:

- `LCD_BL = 38`
- `ADKEY_OUT_PIN = 2` for the Keyes ADKeyboard resistor ladder
- `TEMP_SENSOR_PIN = 3` for the DS18B20 data line
- GPIO4 is reserved as `LCD_BAT_VOLT` for battery-voltage measurement and must not be used by the sensor.

These should remain aligned with the display and button wiring on the actual board. If the hardware changes, update the sketch and the relevant documentation together.

## TFT_eSPI notes

Use the `Setup206_LilyGo_T_Display_S3.h` configuration rather than guessing pin mappings. This includes the correct SPI and timing settings for the LilyGo T-Display S3's ST7789 panel.

When editing display-related code:

- check the active selected setup in `User_Setup_Select.h`
- verify the board setup file for SPI pins and driver configuration
- avoid changing display pins without updating the relevant `TFT_eSPI` setup

## Developer expectations

- Keep the sketch modular and readable.
- Prefer `millis()`-based timing instead of blocking delays.
- Keep hardware pin definitions centralized and easy to adjust.
- Update documentation when changing sensors, display behavior, or inputs.
- Validate compile/upload workflows after adding features.

## Build / upload guidance

Use the correct ESP32-S3 board package and board selection in the Arduino IDE or Arduino CLI.

Example:

```bash
arduino-cli compile --fqbn <YOUR_ESP32S3_FQBN> ./
arduino-cli upload -p COM3 --fqbn <YOUR_ESP32S3_FQBN> ./
```

Replace the FQBN and COM port with the exact target board and port used in the environment.

## Default working assumptions

- This is an ESP32-S3 project.
- The display is a LilyGo T-Display S3 with ST7789 170x320 panel.
- The active display config is `Setup206_LilyGo_T_Display_S3.h`.
- The sketch is still a UI prototype and can be extended with sensors and control logic.

## Agent behavior

When making changes, the agent should:

1. Keep edits focused and minimal.
2. Preserve the current TFT_eSPI display configuration unless a hardware change requires different pins.
3. Update `README.md` and project notes when behavior or setup changes.
4. Prefer root-cause fixes and tested changes.

This file defines the workspace-level rules for agents working in this repository.
