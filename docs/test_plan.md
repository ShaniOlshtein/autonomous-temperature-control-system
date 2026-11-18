# Bench test plan (the Lead maintains it from `qa-safety` reports)

All tests run WITHOUT the heater connected: use an LED or the relay alone. The human compiles, uploads and reports.

## Stage 0 — module split (no behavior change)
Run the checklist in `docs/stage0_refactor.md` before and after. Both runs must look and behave the same.
qa-safety review (2026-11-18): APPROVED FOR COMPILE AND BENCH TEST, NOT COMPILED. Detailed checklist from that review:
1. Boot: black background, grid "Timer / Target Temp / Current Temp", timer OFF red, target 65C HOT red; Serial `OneWire devices found: 2`, then `ADC=… decoded=SW0` about every 250 ms.
2. The selected field blinks about every 0.4 s.
3. SW4 / SW1 move forward / back through the 4 fields with wrap-around; Serial `Button state: SW4` / `SW1` / `SW0`.
4. SW2 / SW3: timer ON/OFF toggles; timer value 1 s per press, hold = 60 s step after 0.6 s then every 0.25 s, stops at 00:00; target 1 degree per press, 5 when held, not below 0; HOT/COLD toggles (COLD cyan).
5. SW5 short press: background dark green at once; Serial `SW5 press: START`.
6. SW5 again while green: one full redraw (quirk E2, expected).
7. SW5 held 1.2 s: green then black; Serial START then `SW5 long press: STOP`.
8. Countdown: only with timer ON and green background, 1 per second, stops at 00:00.
9. Sensors: per-sensor lines and `Valid temperature sensors: 2/2` about every 2 s; screen shows the average as a whole number.
10. One sensor unplugged: `DISCONNECTED`, `1/2`, a value still shown.
11. Both unplugged: `0/2`, screen `ERR`; plug back: values return within about 2 s.
12. Reset (EN) while green and counting: boots black, timer OFF, 03:50 when switched ON.
13. Power cycle: same as 12.
14. Over-temperature and BLE tests: N/A in Stage 0 (no relay, limits or BLE).
15. Compare with the old firmware: no new flicker, no new or missing Serial lines.

## Stage 1 — control
Boot state (relay OFF) · temperature below target turns the relay ON · reaching the target turns it OFF · hysteresis (no chatter) · unplug sensor 1 · unplug sensor 2 · unplug both (relay OFF and ERR shown) · heat a sensor above the hard maximum (lockout) · reset while ON (comes back OFF) · power cycle.

## Stage 2 — settings
Change values, power-cycle, values persist · out-of-range values are rejected · factory reset · timer duration is saved, remaining time is not.

## Stage 3 — BLE (nRF Connect)
Connect · read all characteristics · write valid and invalid values · notifications arrive · disconnect during heating (control continues) · unbonded write is rejected · local change on the buttons is notified to the phone.

## Stage 4 — display
Values match BLE · BLE status shown · fault shown.

Only after all stages: a supervised test with the real heater, by a person qualified for mains wiring.

## Results log
(The Lead adds the date, stage and what the human reported.)
- 2026-11-18 — Stage 0: compile and upload OK; buttons not responding (first report).
- 2026-11-19 — Stage 0: human reports the stage passed successfully (compile, upload, bench test). Approved for commit and push.
