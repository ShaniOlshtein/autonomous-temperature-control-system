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
8. **Behavior after boot or power loss**: stay stopped (recommended) or resume the previous run? The current code boots with cooking inactive and the timer OFF (see E12).
9. **Relay pins**: confirm that GPIO10 (heater) and GPIO11 (cooler) are reachable on your header.
10. **Thermal behavior** (°C per minute heating/cooling): can be measured later, needed for overshoot tuning.

## B. Needed for BLE (Stage 3)
12. **Security**: pairing method (proposal: LE Secure Connections with a passkey shown on the screen) and bonded-only writes.
13. **Phone**: Android, iOS or both? Test first with nRF Connect (no app), custom app later (Flutter, Web Bluetooth or native)?
14. Should the on-device buttons stay enabled while a phone is connected? (Recommended: yes.)
15. Approve `docs/ble_spec.md` (wire format; the UUIDs were generated randomly for this project).

## C. Environment and repo
20. Git: base branch, remote and pull request policy (repo `ShaniOlshtein/autonomous-temperature-control-system`).
21. Power: USB only, or battery too? (Battery uses GPIO4 monitoring and affects the backlight.)
22. Display: temperatures with decimals (0.1 °C) or whole degrees (current)?
23. Units: Celsius only?

## D. Inconsistencies between the summary document and the code
- The summary says the relay/heater logic exists; **the code has no relay and no setpoint comparison** (UI prototype only).
- The summary says the loop runs "many times per second"; sensors are read every 2 s (fine for brewing; confirm).
- README lists `currentTemp` as a configuration variable; it is a measurement.
- The ASCII flow in the summary has no fault or sensor-error branch.

## E. Current quirks kept unchanged in Stage 0 (decision 15) — decide the fix in Stage 1
Found in `cooking_temperature_control.ino` (479 lines, `main` 43e3339). For each one: keep, fix, or change.
- E1. **SW5 hold from stopped**: START fires on press, then STOP fires after 1.2 s. Holding SW5 to stop always passes through START first.
- E2. **SW5 press while already active**: redraws the whole static screen again (possible flicker) and resets the one-second timestamp, so the current second restarts.
- E3. **Timer ON while cooking is already active**: `previousMillis` is only set at START, so the first decrement can happen immediately.
- E4. **Timer reaches 0**: nothing happens. It stays at 00:00, cooking stays active, no beep or stop.
- E5. **Timer value has no upper limit** (`int remainingSeconds`); minutes above 99 show 3+ digits. The value can also be edited while counting.
- E6. **Target temperature**: only clamped at 0, no upper limit. Shown as a whole number.
- E7. **HOT/COLD (`isHot`) is cosmetic**: it changes only the label; nothing uses it.
- E8. **Temperatures are truncated, not rounded** (`(int)temp`): 64.9 shows 64; negative values truncate toward zero.
- E9. **Sensor reading by index** (`getTempCByIndex`): sensor order can change after replugging; the 85 °C power-on value of a DS18B20 is not filtered (relevant for safety in Stage 1).
- E10. **Serial output volume**: ADC debug line every 250 ms and per-sensor lines every 2 s.
- E11. **Rendering**: all three sprites are redrawn on every `loop()` pass with no rate limit, and `String` objects are built each pass (heap use).
- E12. **Boot state**: cooking inactive and timer OFF at boot; there is no saved state (ties to question 8).

## F. VS Code toolchain (plan `docs/plans/vscode_toolchain.md`) — open on 2026-11-19
- F1. Approve fix bundle A-D from the qa-safety review (settings.json rules, settings.local.json `git push *`, git-release docs-only check, wording).
- F2. Arduino IDE program-size lines for the same commit (compare with arduino-cli: 365286 bytes, 27%; globals 28360 bytes, 8%).
- F3. Keep `tools/build.ps1` without `-v` (no "Used library" table; versions from `core list`/`lib list`)? Lead recommends yes.
- F4. Is GitHub branch protection enabled on `main`?

## G. Buttons — open on 2026-11-22
- G1. SW5 does not work: human says it is a hardware fault that existed before; she will fix it. After the fix, measure its ADC value (Serial `ADC=...`) to check the SW5 range 3001-3400.
- G2. ADC values of SW1, SW2 and "no button" were never measured on this board (only SW3 1702-1705 and SW4 2789-2793 on 2026-11-22, SW4 about 2664 earlier).
