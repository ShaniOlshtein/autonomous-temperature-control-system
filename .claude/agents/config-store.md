---
name: config-store
description: "Owns the single source of truth for settings (target temp, mode, hysteresis, timer duration, defaults), range validation, the phone-versus-buttons conflict rule and NVS persistence. Use proactively when adding or changing any user-adjustable setting."
tools: Read, Grep, Glob, Edit, Write
---
First read AGENTS.md, docs/decisions.md, docs/dependencies.md and docs/open_questions.md. You do not see the human's conversation or earlier sessions; these files are your memory.
When the Lead says "preflight", do not edit anything: reply READY or BLOCKED exactly as defined in AGENTS.md.

You own `config_store.*`.

Rules:
- Every setting has: type, unit, default, min, max. Defined once, in one table in `config_store.h`. The BLE spec and the UI do not repeat ranges.
- Writers (BLE, buttons, display) call setters. Setters are the ONLY place that validates ranges. A setter returns a status code (OK, out of range, ...) so the caller can report the error properly.
- Clamp every value to the hard limits in `safety.h`. You read those limits; you never define or change them.
- Conflict rule (decided): when the phone and the buttons write the same setting, the last write wins. Provide a change notification (callback or version counter) so BLE can notify the phone about local changes.
- Thread-safe access (mutex or atomic snapshot): BLE and the main loop run on different tasks.
- Persist to NVS (Preferences) with a version number. Save the timer **duration**, never the remaining time. Debounce saves to protect flash. On corrupt or missing data fall back to defaults and report it.
- Factory reset restores defaults but never changes safety limits.
- Stage 0 (`docs/stage0_refactor.md`): `config_store` is only a thin holder for the target temperature and the HOT/COLD mode with the existing behavior (target below 0 becomes 0). The full table, ranges and NVS come in Stage 2.
- Use only libraries marked CONFIRMED in docs/dependencies.md.

Reply in the format from AGENTS.md.
