# Open questions — information still missing (human to answer)

Answered questions move to `docs/decisions.md`. The Lead keeps this file current.

## A. Needed before Stage 1 (relay + control)
1. **Relay module**: model, trigger voltage (3.3V or 5V logic), active-HIGH or active-LOW, mechanical relay or SSR, rated current.
2. **Heater**: type, power (W), voltage (230V?), switched directly or through a contactor.
3. **Cooling**: is there a cooling output at all? What is it (compressor, pump/glycol valve, Peltier, fan)? One relay or two? What does HOT/COLD mean physically?
4. **Hard limits (decision 8)**: hard maximum temperature, hard minimum temperature if there is cooling, and the minimum relay on/off time.
5. **Ranges and defaults — approve or change.** PROPOSED: target 0–100.0 °C, hysteresis 0.2–10.0 °C with default 0.5 °C, timer duration 0–359999 s. Current code defaults: target 65 °C, timer 230 s, mode HOT.
6. **Two sensors**: average (current behavior) or redundancy? Maximum allowed difference before declaring a fault? Where is each probe placed (in the liquid or on the pot wall)?
7. **Timer semantics**: what happens when it reaches 0 (stop heating, beep, keep holding temperature)? Does it start counting on START or only when the target is reached?
8. **Behavior after boot or power loss**: stay stopped (recommended) or resume the previous run? The current code starts the timer running at boot.
9. **Relay pins**: confirm that GPIO10 (heater) and GPIO11 (cooler) are reachable on your header.
10. **Thermal behavior** (°C per minute heating/cooling): can be measured later, needed for overshoot tuning.
11. **Stage 0**: keep the current quirks (timer running at boot, temperatures shown as whole numbers, HOT/COLD only cosmetic) unchanged during Stage 0? Recommended: yes.

## B. Needed for BLE (Stage 3)
12. **Security**: pairing method (proposal: LE Secure Connections with a passkey shown on the screen) and bonded-only writes.
13. **Phone**: Android, iOS or both? Test first with nRF Connect (no app), custom app later (Flutter, Web Bluetooth or native)?
14. Should the on-device buttons stay enabled while a phone is connected? (Recommended: yes.)
15. Approve `docs/ble_spec.md` (wire format; the UUIDs were generated randomly for this project).

## C. Environment and repo
16. Exact Arduino environment and the board settings you use today (record them in `docs/dependencies.md`). Agents do not compile.
17. Exact versions that compile today: Arduino IDE, ESP32 core, TFT_eSPI, OneWire, DallasTemperature.
18. **TFT_eSPI setup lives outside the repo** (`libraries/arduino_853159/...`). Copy the two setup files into the repo or document the steps, so builds are reproducible.
19. **Sketch folder**: the Arduino folder name must equal the main `.ino` name. Which local folder do you compile from? `pins.h` and the new `.h/.cpp` files must sit in that folder, next to the `.ino`.
20. Git: base branch, remote and pull request policy (repo `ShaniOlshtein/autonomous-temperature-control-system`).
21. Power: USB only, or battery too? (Battery uses GPIO4 monitoring and affects the backlight.)
22. Display: temperatures with decimals (0.1 °C) or whole degrees (current)?
23. Units: Celsius only?

## D. Inconsistencies between the summary document and the code
- The summary says the relay/heater logic exists; **the code has no relay and no setpoint comparison** (UI prototype only).
- The summary says the loop runs "many times per second"; sensors are read every 2 s (fine for brewing; confirm).
- README lists `currentTemp` as a configuration variable; it is a measurement.
- The ASCII flow in the summary has no fault or sensor-error branch.
