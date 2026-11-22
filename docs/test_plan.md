# Bench test plan (the Lead maintains it from `qa-safety` reports)

All tests run WITHOUT the heater connected: use an LED or the relay alone. `build-runner` compiles and, only with the human's approval of each upload, uploads; the human runs the checklist and reports.

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

## Toolchain (chore/vscode-toolchain) — first arduino-cli upload, firmware unchanged (Stage 0)
From qa-safety, 2026-11-19 (APPROVED FOR COMPILE AND BENCH TEST). Heater DISCONNECTED; relay module disconnected or LED only (GPIO10/11 are not driven by Stage 0).
1. Before the upload: close every Serial Monitor; build-runner runs `arduino-cli board list`; the human confirms the port; the approval names the .bin (path, size, build time from the latest exit-0 report) and the port.
2. Optional gate test: VS Code Upload task, type a wrong port => "Upload cancelled. Nothing was flashed."
3. Upload: exit code 0. If it cannot connect: hold BOOT, press RST, release BOOT, retry. Press RST if the board does not restart.
4. Serial 115200 (port may re-enumerate): "OneWire devices found: 2", per-sensor lines and "Valid temperature sensors: 2/2" every 2 s, ADC debug line every 250 ms.
5. Display: backlight on, three-column 320x170 layout as with the IDE build (`Simulation.png`).
6. Buttons SW1-SW5 as in the Stage 0 checklist, including the SW5 START/STOP Serial lines.
7. Sensors: unplug one => "DISCONNECTED" and 1/2; unplug both => 0/2; display as with the IDE build.
8. RST and power cycle: boots with cooking inactive and timer OFF (E12); Serial returns (CDC On Boot).
9. Program size compared with an Arduino IDE build of the same commit.
Not testable yet: over-temperature, BLE disconnect while heating, reset while heating (Stage 1/3).

## Results log
(The Lead adds the date, stage and what the human reported.)
- 2026-11-18 — Stage 0: compile and upload OK; buttons not responding (first report).
- 2026-11-19 — Stage 0: human reports the stage passed successfully (compile, upload, bench test). Approved for commit and push.
- 2026-11-22 — Toolchain upload (arduino-cli, COM13): human reports SW3 does not increase the value; it moves the selection forward (acts like a navigation key). Human says SW3 also did not work on 2026-11-19 (correction to the entry above). Pending: VCC of the ADKeyboard (3.3V or 5V) and ADC readings per button from Serial before any threshold change.
- 2026-11-22 — Toolchain bench test (arduino-cli build, one DS18B20 connected): human reports everything passes except SW3 (pre-existing threshold bug, measured ADC 1702-1705 vs limit 1700; SW4 measured 2789-2793) and SW5 (pre-existing HARDWARE fault, human will fix; not a firmware task). Approved for commit, push and PR.
