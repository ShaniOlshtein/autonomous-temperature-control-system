# Worklog (append-only: the Lead adds an entry after every task and never edits old entries)

Entry format:
```
## YYYY-MM-DD — <task>
- Requested by: human
- Delegated to: <agents and their replies, e.g. control-logic (preflight READY), qa-safety (APPROVED FOR COMPILE AND BENCH TEST)>
- Files changed:
- Result: <one sentence>
- Verified / Assumed / Unknown:
- Open items:
```

## 2026-11-18 — Agent team set up
- Requested by: human
- Delegated to: none (setup by the chat assistant)
- Files changed: CLAUDE.md, AGENTS.md, pins.h, .claude/, docs/
- Result: agent team and project documents added; decisions 1-13 recorded
- Verified / Assumed / Unknown: nothing compiled or tested; Claude Code behavior of this setup not yet observed
- Open items: Stage 0; dependency versions; hard limits; answers to `docs/open_questions.md`

## 2026-11-18 — Stage 0 survey and plan update
- Requested by: human
- Delegated to: none (Lead survey and Lead-owned docs only; no branch, no agents)
- Files changed: docs/stage0_refactor.md, docs/open_questions.md (removed Q11, added section E quirks), docs/decisions.md (14-18), docs/worklog.md
- Result: Stage 0 mapping rewritten for the 479-line sketch on `main`; every variable, constant and function mapped to a module
- Verified / Assumed / Unknown: mapping checked by reading only; nothing compiled; owners still must confirm the mapping in preflight
- Open items: human approval of the updated plan; then branch and preflight

## 2026-11-18 — Stage 0 branch, preflight and contracts
- Requested by: human (plan approved)
- Delegated to: git-release (branch `refactor/stage0-modules` from main 43e3339), hardware-pins (preflight READY, pins.h unchanged), config-store (READY, wrote config_store.h), control-logic (READY, wrote sensors.h, timer.h), display-ui (READY, wrote display_ui.h, input.h)
- Files changed: config_store.h, sensors.h, timer.h, display_ui.h, input.h (new, headers only); docs/open_questions.md (A8 corrected)
- Result: 25 public functions agreed with a module-prefix naming rule; cookingActive/uiNeedsRedraw split by ordered calls from input (no redraw timing change); backlight split from display init to keep setup order
- Verified / Assumed / Unknown: headers checked by reading only; NOT COMPILED; whether Setup206 also defines LCD_BL is unknown (identical redefinition expected to be harmless, as today)
- Open items: human approval of the contracts; then implementation

## 2026-11-18 — Stage 0 implementation and review
- Requested by: human (contracts approved, decision 19)
- Delegated to: config-store (config_store.cpp), control-logic (sensors.cpp, timer.cpp, slim .ino), display-ui (display_ui.cpp, input.cpp), qa-safety (APPROVED FOR COMPILE AND BENCH TEST, no behavior change found)
- Files changed: cooking_temperature_control.ino (479 → 36 lines), config_store.cpp, sensors.cpp, timer.cpp, display_ui.cpp, input.cpp (new); .claude/agents/control-logic.md (timerRunning line fixed at the human's request); docs/stage0_refactor.md (approved contracts section), docs/test_plan.md (Stage 0 checklist), docs/decisions.md (19)
- Result: sketch split into 5 modules; qa-safety listed 15 differences, all moves/renames or low build risks, none behavioral
- Verified / Assumed / Unknown: NOT COMPILED, not bench-tested; low build risks: `timer.h` name vs ESP32 core, static-init order across files, library discovery from .cpp; ESP32 core version "1.8.3" in dependencies.md looks wrong
- Open items: human compile + bench test; ESP32 core version; who changed `.claude/agents/git-release.md` (`model: haiku`); then commit + PR via git-release

## 2026-11-18 — Stage 0 bench test, first report
- Requested by: human
- Delegated to: none (Lead read input.cpp and the .ino to diagnose)
- Files changed: docs/worklog.md
- Result: human reports compile and upload OK, but the buttons do not respond. Button path (GPIO2, ADC 11 dB, thresholds, debounce, handlers, loop order) matches the original sketch line for line by reading
- Verified / Assumed / Unknown: compile and upload OK (human); buttons not working (human); cause unknown; not yet known whether the buttons worked with the original firmware
- Open items: Serial Monitor output (ADC lines); test the original firmware from main for comparison; no commit until resolved

## 2026-11-18 — VS Code toolchain: research and plan (no changes)
- Requested by: human
- Delegated to: none (Lead research only; no install, no code, no agents, no branch)
- Files changed: docs/worklog.md
- Result: local survey (Arduino IDE prefs: board esp32s3, CDCOnBoot=cdc, USBMode=hwcdc, PSRAM=disabled, FlashSize=4M, PartitionScheme=default, port COM11; core 3.3.11; libs TFT_eSPI 2.5.43, OneWire 2.3.8, DallasTemperature 4.0.6; PlatformIO extension 3.3.4 + espressif32 7.0.1 = Arduino core 2.0.17 already installed; arduino-cli not installed). Researched arduino-cli profiles vs PlatformIO; recommendation arduino-cli + ASCII working folder outside OneDrive
- Verified / Assumed / Unknown: local facts read from files; nothing compiled with the new tools; Stage 0 PR not merged (buttons issue open), so chore/vscode-toolchain cannot be created yet
- Open items: human decisions listed in the plan

## 2026-11-19 — Stage 0 bench test passed; commit and push
- Requested by: human ("Stage 0 passed successfully, push it to GitHub")
- Delegated to: git-release (commit on `refactor/stage0-modules`, push, PR to main)
- Files changed: docs/worklog.md, docs/test_plan.md (results log), docs/dependencies.md (ESP32 core note)
- Result: human confirms Stage 0 compile, upload and bench test passed; the earlier buttons issue is resolved per her report
- Verified / Assumed / Unknown: Verified by the human on hardware; cause of the earlier buttons issue Unknown (not reported); `.claude/agents/git-release.md` (`model: haiku`) left out of the commit until the human confirms it is hers
- Open items: PR review and merge by the human; then the VS Code toolchain decisions
