# BLE GATT specification (DRAFT — needs the human's approval before implementation)

Owner: ble-service. This file defines the **wire format and security only**.
Ranges and defaults are NOT here: they live only in `config_store.h` (approved values are recorded in `docs/decisions.md`).

Device name: `BrewCtl-XXXX` (XXXX = last 2 bytes of the MAC). Stack: NimBLE-Arduino (proposed; version pending, see `docs/dependencies.md`).
All multi-byte values are **little-endian**. Temperatures are **int16 in 0.1 °C** (650 = 65.0 °C). Invalid or missing reading = 0x7FFF.

Primary service UUID: `68fda9ea-ea0f-44dd-bbcd-6dbcf317ff2a`

| Characteristic | UUID | Props | Format | Meaning |
|---|---|---|---|---|
| Target temp | 9f146af9-ddd7-4c7a-9df9-2fb9808a7ffd | R/W | int16 (0.1 °C) | setting |
| Mode | fd8eafb4-ef11-4a40-9714-8977557c1820 | R/W | uint8 | 0 = HEAT, 1 = COOL |
| Hysteresis | 6c81fd81-df6e-462d-95c9-82d2d4b51c75 | R/W | uint16 (0.1 °C) | setting |
| Timer duration | 70a67ebe-6c6c-4be1-9140-d23b1d594977 | R/W | uint32 (seconds) | setting (saved to flash) |
| Timer remaining | 7493de62-af67-4912-b03e-75251a86ea47 | R/N | uint32 (seconds) | runtime state, read-only |
| Command | 137371dc-8158-4ef0-a5e7-efb9f0619207 | W | uint8 | 1 = START, 2 = STOP, 3 = RESET TO DEFAULTS |
| Current temp | b24fb434-8f3c-4695-b412-4df1e6e316f1 | R/N | int16 (0.1 °C) | average of the valid sensors |
| Status | dd030e26-241b-4e5c-98cd-0f2d645ba4fb | R/N | uint16 bitmask | see below |

Status bits: 0 sensor 1 fault · 1 sensor 2 fault · 2 sensors disagree · 3 over-temperature lockout · 4 heater ON · 5 cooler ON · 6 running · 7 timer finished.

## Write rules
- Wrong attribute length or type => ATT error "Invalid Attribute Value Length". This is the only check BLE performs.
- Out-of-range value => `config_store` rejects it; BLE maps the returned status to ATT "Value Not Allowed". The setting is unchanged. (Verify the exact NimBLE constant names in the CONFIRMED library version.)
- Writes require an encrypted and bonded link. Pairing method: PENDING human decision (proposal: LE Secure Connections, passkey shown on the screen).
- Writes are rate-limited.

## Notifications
- Current temp: every 2 s, or on change of 0.1 °C or more.
- Timer remaining: once per second while running.
- Status: on change.
- Settings changed locally (buttons/display): notify the new value.

## Open items (see `docs/open_questions.md`)
Hard maximum temperature and range values are not defined here and are still UNKNOWN. Pairing method is pending.
