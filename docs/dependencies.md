# Dependencies (libraries and versions)

Status: **CONFIRMED** (the human installed it and it compiled) · **REQUESTED** (an agent needs it, not installed yet) · **UNKNOWN**
Only the Lead edits this file. Find installed versions in Arduino IDE: Tools > Manage Libraries (filter: Installed) and Tools > Board > Boards Manager.

| Library | Version | Needed by | Status | Notes |
|---|---|---|---|---|
| ESP32 core (esp32 by Espressif Systems) | 1.8.3 | all | CONFIRMED | human to fill in. Official LilyGO note: newer cores may break TFT_eSPI |
| Arduino IDE | 1.8.19 | human | CONFIRMED | LilyGO note: above 2.0.14 may break TFT_eSPI |
| TFT_eSPI | 2.5.43 | display-ui | CONFIRMED | requires `Setup206_LilyGo_T_Display_S3.h` selected in `User_Setup_Select.h` |
| OneWire | 2.3.8 | control-logic | CONFIRMED | already used by the sketch |
| DallasTemperature | 4.0.6 | control-logic | CONFIRMED | already used by the sketch |
| NimBLE-Arduino | - | ble-service | REQUESTED | proposed for BLE. 1.x and 2.x APIs differ: no BLE code before the version is CONFIRMED |

The libraries that the sketch already uses are probably installed, since it compiles for the human today. They become CONFIRMED once she reports their versions.
