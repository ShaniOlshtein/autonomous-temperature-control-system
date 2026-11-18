#pragma once
// Single source of truth for GPIO assignments. Owner: hardware-pins agent.
// Facts and evidence: docs/hardware.md (official LilyGO T-Display-S3 pin_config.h and pinmap).
// This file defines NO display macros: the TFT_eSPI setup (Setup206_LilyGo_T_Display_S3.h) owns the display pins.

// ---- Reserved pins (documentation only — DO NOT USE for anything else)
// Display: data D0..D7 = 39,40,41,42,45,46,47,48 | RST=5 CS=6 DC=7 WR=8 RD=9
// Board: 15 (power on), 4 (battery voltage), 0 (BOOT button), 14 (second onboard button)
// Not usable: 19/20 (native USB), 26..37 (flash / octal PSRAM)

// ---- Pins used by the sketch (CONFIRMED; same names as the existing code)
#define LCD_BL            38   // display backlight (the sketch drives it directly)
#define ADKEY_OUT_PIN      2   // ADKeyboard analog out (ADC1_CH1), read with 11 dB attenuation
#define TEMP_SENSOR_PIN    3   // OneWire data for the DS18B20 sensors (ADC1_CH2; strapping pin, see docs/hardware.md)
#define TEMP_SENSOR_COUNT  2   // number of DS18B20 sensors on the shared bus

// ---- Relay outputs (PROPOSED — NOT CONFIRMED; the human must confirm pins and relay module)
#define RELAY_HEATER_PIN  10   // PROPOSED
#define RELAY_COOLER_PIN  11   // PROPOSED, only if a cooling output is built
// The relay active level is UNKNOWN (many relay modules are active-LOW).
// Define it only after the human confirms the module:
// #define RELAY_ACTIVE_LEVEL HIGH
