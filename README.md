# Cooking Temperature Control (Arduino)

A simple Arduino project that displays a countdown timer, target temperature, and current temperature on a TFT display. Designed as a small control/monitoring UI for cooking applications (e.g., brewing), it uses the `TFT_eSPI` library and sprite rendering for smooth updates.

## Features
- Countdown timer with ON/OFF state
- Display of target temperature and current temperature
- Simple button inputs (left/right) for future interaction

## Hardware
- ESP32-S3 LilyGo T-Display S3
- ST7789 170x320 TFT display
- Two buttons (configured with internal pull-ups)

## Wiring (as used in the sketch)
- LCD backlight: pin `38` (`LCD_BL`)
- Left button: pin `0` (`BTN_LEFT`) — uses `INPUT_PULLUP`
- Right button: pin `14` (`BTN_RIGHT`) — uses `INPUT_PULLUP`
- DS18B20 data: pin `3` (`TEMP_SENSOR_PIN`)

GPIO `4` must not be used for the temperature sensor because the board uses it
for `LCD_BAT_VOLT` battery-voltage measurement. For the DS18B20, connect `VDD`
to `3.3V`, `GND` to `GND`, and `DQ` to GPIO `3`.
Add a `4.7 kOhm` pull-up resistor between `DQ` and `3.3V`. Do not connect the
sensor data line to the display or button pins.

Adjust wiring depending on your board (pin numbers may differ).

## Installation and Build

Install the following before opening or compiling the sketch:

- Arduino IDE 2.x or Arduino CLI
- ESP32 board package by Espressif Systems
- `TFT_eSPI` — TFT and sprite drawing
- `OneWire` — DS18B20 data protocol
- `DallasTemperature` — DS18B20 temperature readings

`SPI` is included with the Arduino platform and does not need a separate installation.

The project uses the LilyGo T-Display S3 configuration in
`libraries/arduino_853159/User_Setup_Select.h`, which selects
`User_Setups/Setup206_LilyGo_T_Display_S3.h`. Do not replace this setup with a
generic display configuration.

### Arduino IDE

Install the ESP32 board package and the three libraries above using the IDE's
Boards Manager and Library Manager. Select the LilyGo T-Display S3 compatible
ESP32-S3 board and the connected serial port, then open
`cooking_temperature_control.ino` and use Verify or Upload.

### Arduino CLI

Arduino CLI can use the same locally installed board package and libraries as
the Arduino environment. First identify the board and port instead of guessing:

```powershell
arduino-cli board list
arduino-cli board listall "LilyGo T-Display S3"
```

Then compile with the exact FQBN reported for the selected board:

```powershell
arduino-cli compile --fqbn <ESP32_S3_FQBN> .
```

Upload only after confirming the connected port:

```powershell
arduino-cli upload -p <PORT> --fqbn <ESP32_S3_FQBN> .
```

The CLI does not automatically use the board and port currently selected in
the Arduino IDE, so both values must be verified explicitly.

## Configuration
Open `cooking_temperature_control.ino` and modify these variables as needed:
- `remainingSeconds` — initial countdown time in seconds
- `targetTemp` — desired target temperature (float)
- `currentTemp` — current measured temperature (float)

## Usage
1. Build and upload the sketch to your Arduino board.
2. Power the TFT display and the board.
3. The screen shows the Timer, Target Temp, and Current Temp columns. The timer counts down when `timerRunning` is true.

## Notes and Next Steps
- Buttons are configured but not wired to actions; you can add handlers to change `targetTemp` or start/stop the timer.
- The DS18B20 is read on GPIO `3` every two seconds using non-blocking conversion timing.
- If the sensor is disconnected or unavailable, the Current Temp field displays `ERR`.

---
Created from the `cooking_temperature_control.ino` sketch in this repository.
