# Bench test plan (the Lead maintains it from `qa-safety` reports)

All tests run WITHOUT the heater connected: use an LED or the relay alone. The human compiles, uploads and reports.

## Stage 0 — module split (no behavior change)
Run the checklist in `docs/stage0_refactor.md` before and after. Both runs must look and behave the same.

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
