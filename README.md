# Cooking Temperature Control (Arduino)

A simple Arduino project that displays a countdown timer, target temperature, and current temperature on a TFT display. Designed as a small control/monitoring UI for cooking applications (e.g., brewing), it uses the `TFT_eSPI` library and sprite rendering for smooth updates.

## Features
- Countdown timer with ON/OFF state
- Display of target temperature and current temperature
- Visual HOT mode label under the target temperature
- ADKeyboard navigation with blinking selection feedback

## Hardware
- ESP32-S3 LilyGo T-Display S3
- ST7789 170x320 TFT display
- Keyes ADKeyboard resistor-ladder module

## Wiring (as used in the sketch)
- LCD backlight: pin `38` (`LCD_BL`)
- ADKeyboard `OUT`: pin `2` (`ADKEY_OUT_PIN`) — ADC1 input with 11 dB attenuation
- DS18B20 data: pin `3` (`TEMP_SENSOR_PIN`)

GPIO `4` must not be used for the temperature sensor because the board uses it
for `LCD_BAT_VOLT` battery-voltage measurement. For the DS18B20, connect `VDD`
to `3.3V`, `GND` to `GND`, and `DQ` to GPIO `3`. Add a `4.7 kOhm` pull-up
resistor between `DQ` and `3.3V`. Do not connect the sensor data line to the
display or button pins.

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
3. The screen shows the Timer, Target Temp, and Current Temp columns. The target column includes a visual HOT/COLD selector with HOT selected. The timer counts down when `timerRunning` is true.
4. Press `SW4` on the ADKeyboard to move the selection forward: `ON/OFF` -> timer value -> target temperature -> `HOT/COLD` -> `ON/OFF`. Press `SW1` to move backward through the same order. The selected part blinks.
5. `SW2` decreases the selected value and `SW3` increases it. On the timer field, a short press changes one second and holding changes one minute at a time. On the target temperature field, a short press changes one degree and holding changes five degrees at a time. On the ON/OFF and HOT/COLD fields, either button toggles the selected state.

The dynamic value areas are inset from the white column and header dividers so
sprite refreshes do not cover the static frame.

## Notes and Next Steps
- The ADKeyboard uses SW1 and SW4 for navigation. SW2 decreases the selected
	timer or target value, and SW3 increases it. SW5 is decoded but has no action
	yet.
- The ADKeyboard resistor ladder is connected to `3.3V` through its internal pull-up. The current approximate decoded ADC ranges are SW1 `0-300`, SW2 `301-950`, SW3 `951-1700`, SW4 `1701-3000` (measured around `2664` on this board), SW5 `3001-3400`, and no button above `3400`.
- The timer ON/OFF and HOT/COLD fields can be changed with SW2 or SW3 when
	selected. Holding an action button repeats timer changes in one-minute steps
	or target-temperature changes in five-degree steps.
- The DS18B20 is read on GPIO `3` every two seconds using non-blocking
	conversion timing.
- If the sensor is disconnected or unavailable, the Current Temp field displays
	`ERR`.

---
Created from the `cooking_temperature_control.ino` sketch in this repository.
