# Hardware reference (source of truth for pins)

Sources: official LilyGO repo `Xinyuan-LilyGO/T-Display-S3` (`examples/*/pin_config.h`, `image/T-DISPLAY-S3.jpg`, `platformio.ini`, README) and TFT_eSPI `User_Setups/Setup206_LilyGo_T_Display_S3.h`.
Rows marked **(general ESP32-S3 knowledge)** are NOT from the LilyGO files: verify them against the Espressif datasheet before relying on them. Anything not listed here is unknown.

## Board
- LilyGO T-Display-S3: ESP32-S3R8 (16MB flash, 8MB OPI PSRAM), 1.9" ST7789 170x320, **8-bit parallel** display, USB-C native USB, BLE 5 + WiFi.
- Arduino settings (official LilyGO README): USB CDC On Boot = Enabled, Flash Mode QIO 80MHz, Partition "16M Flash (3M APP/9.9MB FATFS)", PSRAM = OPI PSRAM, Upload Mode = UART0/Hardware CDC, Core Debug Level None.
- Official note: Arduino IDE above 2.0.14 and newer ESP32 cores may break TFT_eSPI (TFT_eSPI issue #3329). Record the working versions in `docs/dependencies.md`.
- If CDC On Boot is disabled, Serial moves to GPIO43/44.
- The display is configured by TFT_eSPI `Setup206_LilyGo_T_Display_S3.h`, selected in `User_Setup_Select.h`. Both live outside this repo (see open question about reproducible builds). `pins.h` does not define display macros.

## Pins used by the board — NEVER use for anything else
| GPIO | Function | Source |
|---|---|---|
| 38 | LCD backlight (the sketch drives it as `LCD_BL`) | LilyGO |
| 39,40,41,42,45,46,47,48 | LCD data D0..D7 | LilyGO |
| 5,6,7,8,9 | LCD RST, CS, DC, WR, RD | LilyGO |
| 15 | LCD/board power-on enable | LilyGO |
| 4 | Battery voltage measurement (LCD_BAT_VOLT) | LilyGO |
| 0 | BOOT button (strapping) | LilyGO |
| 14 | Second onboard button | LilyGO |
| 19, 20 | Native USB D-/D+ (not on the header) | general ESP32-S3 knowledge |
| 26–37 | Internal flash / octal PSRAM | general ESP32-S3 knowledge |

## Broken out on the headers (candidates)
| GPIO | Notes from the official pinmap | Status in the project |
|---|---|---|
| 1 | ADC1_CH0, touch1 | free |
| 2 | ADC1_CH1, touch2 | **used**: ADKeyboard OUT (`ADKEY_OUT_PIN`) |
| 3 | ADC1_CH2, touch3. ESP32-S3 strapping pin (JTAG source) — general knowledge | **used**: DS18B20 OneWire (`TEMP_SENSOR_PIN`). Works with the 4.7k pull-up in practice; verify against the ESP32-S3 datasheet |
| 10 | ADC1_CH9, SPI_CS, touch10 | free — proposed `RELAY_HEATER_PIN` |
| 11 | ADC2_CH0, SPI_D, touch11 | free — proposed `RELAY_COOLER_PIN` |
| 12 | ADC2_CH1, SPI_CLK, touch12 | free on the official pinmap, but a third-party site marks GPIO12 as flash clock: avoid for relays until verified |
| 13 | ADC2_CH2, SPI_Q, touch13 | free |
| 16 | ADC2_CH5; touch INT on the Touch variant | free on the non-touch board |
| 17 | I2C SCL / U1TXD, ADC2_CH6 | free (reserved for I2C by convention) |
| 18 | I2C SDA / U1RXD, ADC2_CH7 | free (reserved for I2C by convention) |
| 21 | touch RES on the Touch variant | free on the non-touch board |
| 43, 44 | UART0 TX/RX, also on the 4-pin JST | free when CDC On Boot is enabled |

Power pins on the headers: 3V, 5V, GND. GPIO levels are **3.3V**, low current.

## Rules for new pins
- Prefer ADC1 pins (1, 2, 3) for analog; ADC2 conflicts with WiFi (not used now, but keep the option).
- Output pins must be LOW/safe at boot and reset. Check strapping behavior first.
- A mechanical relay module is usually 5V-powered with a transistor/optocoupler input and may be active-LOW. Never drive a coil from a GPIO directly. Add a flyback diode if the module does not have one.
- Mains (230V): fuse or breaker + RCD, enclosed box, independent hardware thermal cutoff. This is not a software job.

## Current wiring (from README)
- ADKeyboard OUT -> GPIO2 (ADC, 11 dB). Decoded ranges: SW1 0-300, SW2 301-950, SW3 951-2250 (about 1703 measured 2026-11-22), SW4 2251-3000 (about 2790 measured 2026-11-22; about 2664 earlier), SW5 3001-3400, no button above 3400.
- 2x DS18B20: VDD -> 3.3V, GND -> GND, DQ -> GPIO3 (shared), one 4.7k pull-up between DQ and 3.3V.
