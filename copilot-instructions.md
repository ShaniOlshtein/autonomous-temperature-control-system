# Copilot instructions for Cooking Temperature Control

## Project overview
This workspace contains an Arduino sketch for a cooking temperature control UI for an ESP32-S3 LilyGo T-Display board. The project uses `TFT_eSPI` and the active display configuration is the LilyGo T-Display S3 setup.

## Hardware baseline
- Board: ESP32-S3
- Display: LilyGo T-Display S3
- Panel: ST7789, 170x320 TFT
- Active TFT_eSPI setup: `libraries/arduino_853159/User_Setups/Setup206_LilyGo_T_Display_S3.h`
- Selected in: `libraries/arduino_853159/User_Setup_Select.h`

## Required project conventions
- Treat `Setup206_LilyGo_T_Display_S3.h` as the source of truth for display wiring and settings.
- Do not change display pins or TFT settings without checking the selected `TFT_eSPI` setup.
- Keep pin definitions centralized in the sketch (`LCD_BL`, `BTN_LEFT`, `BTN_RIGHT`).
- Prefer `millis()`-based timing instead of blocking delays.
- Keep code modular and readable.

## Current GPIO mapping in the sketch
- `LCD_BL = 38`
- `BTN_LEFT = 0`
- `BTN_RIGHT = 14`

These should stay aligned with the actual board wiring. If hardware changes, update both the sketch and the `TFT_eSPI` setup together.

## Documentation expectations
- Update `README.md` when features, wiring, or behavior change.
- Keep notes and setup details in English.
- When adding sensors or inputs, explain wiring and configuration clearly.

## Build and upload workflow
Use the correct ESP32-S3 board package and target board selection.

Example:

```bash
arduino-cli compile --fqbn <YOUR_ESP32S3_FQBN> ./
arduino-cli upload -p COM3 --fqbn <YOUR_ESP32S3_FQBN> ./
```

Replace the FQBN and COM port with the board and port used in this environment.

## Working style
- Keep changes focused and minimal.
- Prefer root-cause fixes over broad refactors.
- Validate after edits, especially when changing display or GPIO-related code.
- When a hardware or library setup is uncertain, check the active `TFT_eSPI` config before guessing.
