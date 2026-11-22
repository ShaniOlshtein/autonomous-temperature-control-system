# Firmware architecture (target design)

```
 BLE (phone)      display + buttons
      \                /
       +--> config_store  (settings table, range validation, NVS, last-write-wins)
                  |  validated settings (clamped to safety.h limits)
                  v
 sensors --> thermostat --> safety --> relays (heater / cooler)
 timer  -----------^
```

## Modules and owners
All module files (`.ino`, `.cpp`, `.h`) live in the sketch folder `cooking_temperature_control/`.

| Module | Owner | Responsibility | Stage |
|---|---|---|---|
| `pins.h` | hardware-pins | GPIO names | 0 |
| `sensors.*` | control-logic | DS18B20 read, average, fault detection | 0 |
| `timer.*` | control-logic | runtime state: remaining time, running | 0 |
| main `.ino` | control-logic | `setup()` / `loop()` call init/update of each module | 0 |
| `config_store.*` | config-store | thin holder in Stage 0; settings table, ranges, NVS from Stage 2 | 0 / 2 |
| `display_ui.*`, `input.*` | display-ui | screen and buttons | 0 |
| `thermostat.*`, `safety.*` | control-logic | hysteresis, relays, hard limits, watchdog | 1 |
| `ble_service.*` | ble-service | GATT, security, notifications | 3 |

## Rules
- BLE, display and buttons only call `config_store` setters. Only thermostat and safety touch relays.
- The control loop keeps working without BLE. Settings are applied at the next control tick.
- Range validation: only in `config_store` setters (they return a status code). Hard limits: constants in `safety.h`; `config_store` clamps to them but never defines them. Both layers protect independently.
- Setting vs state: the timer **duration** is a setting; the **remaining time** is runtime state in `timer.*`. Only the duration is saved to flash.
- Conflict rule: last write wins (`config_store`); local changes are notified to BLE.
- Init order in `setup()`: safety/relays OFF first, then config, sensors, timer, display, input, BLE.
- Tasks: the control loop is highest priority and never blocks; BLE runs in its own task (NimBLE); UI refresh runs in the main loop.
- Pins come only from `pins.h`.

## Stages
0. Split the current sketch into modules, no behavior change (`docs/stage0_refactor.md`).
1. Control with relay, hysteresis, fail-safe (no BLE).
2. `config_store` complete (table, NVS, timer duration vs remaining).
3. BLE service.
4. Display update.
5. Phone app (later, separate decision).

## Serial (USB-CDC) rule
`setup()` sets `Serial.setTxTimeoutMs(0)` (decision 37): Serial writes never block `loop()`; output may be dropped when no host reads. Do not call `Serial.flush()`: with timeout 0 it discards pending output (core 3.3.11 `HWCDC.cpp` flush). Print only from the loop task; with timeout 0 a write from a second task can be dropped when the TX lock is held.
