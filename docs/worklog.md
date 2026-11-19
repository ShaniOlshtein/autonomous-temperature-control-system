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

## 2026-11-19 — Human answers after the Stage 0 push
- Requested by: human
- Delegated to: none
- Files changed: docs/decisions.md (20), docs/worklog.md
- Result: `model: haiku` in git-release.md is the human's intentional change (decision 20); the earlier "buttons not responding" report was a human test mistake, not a code issue; the human opens the Stage 0 PR on the GitHub website
- Verified / Assumed / Unknown: Verified by the human's statements; Stage 0 commit 0239b6b pushed; PR not yet opened
- Open items: commit git-release.md (separately); merge of the Stage 0 PR; then the VS Code toolchain decisions

## 2026-11-19 — Stage 0 merged; VS Code toolchain decisions recorded
- Requested by: human
- Delegated to: none (Lead-owned docs only; no code, no install)
- Files changed: docs/decisions.md (21-27), docs/open_questions.md (16, 17, 19 removed; 18 kept until the setup file is copied), docs/plans/vscode_toolchain.md (new), docs/worklog.md
- Result: Stage 0 is complete and merged to `main` (PR #3, merge commit a14f0a8); the buttons were bench-tested by the human and work. This supersedes the "PR not merged" note in the 2026-11-18 toolchain entry. Toolchain decisions recorded: arduino-cli with core 3.3.11 (21, supersedes 3), locked board settings (22), no 16MB/OPI before Stage 3 (23), working folder `C:\dev\cooking_temperature_control` (24), build-runner agent (25), upload only with approval (26), reporting rule (27). Plan saved to `docs/plans/vscode_toolchain.md`
- Verified / Assumed / Unknown: Verified: `main` = origin/main = a14f0a8 (git log). Verified: `C:\dev\cooking_temperature_control` does not exist yet (plan step 1, human). Unknown: FQBN option keys for the locked settings (build-runner checks with `arduino-cli board details`). Nothing compiled
- Open items: plan step 1 (human installs arduino-cli and clones); step 2 branch; docs commit vs decision 27 (see plan, open points)

## 2026-11-19 — Docs-only commit exception; working folder check
- Requested by: human
- Delegated to: none (Lead-owned docs only)
- Files changed: docs/decisions.md (28), docs/worklog.md
- Result: decision 28 recorded (docs-only commits allowed without a compile output; decision 27 kept). The human reported moving the repo to `C:\dev\cooking_temperature_control`, but that folder did not exist; the human then confirmed the only repo is `~/OneDrive/Arduino/cooking_temperature_control`. Work continues there
- Verified / Assumed / Unknown: Verified: `C:\dev` does not exist; OneDrive repo on `chore/vscode-toolchain` with today's changes. Unknown: whether decision 24 (working folder outside OneDrive) still stands
- Open items: human to confirm the docs-only commit list; human to confirm or change decision 24

## 2026-11-19 — Working folder decided; docs-only commit
- Requested by: human (commit approved; option B for the folder)
- Delegated to: git-release (docs-only commit on `chore/vscode-toolchain`, decision 28)
- Files changed: docs/decisions.md (29), docs/plans/vscode_toolchain.md (folder, step 1, progress), docs/worklog.md
- Result: the repo stays in OneDrive (decision 29 amends 24); today's docs committed on the branch, no push
- Verified / Assumed / Unknown: Verified: diff contains docs only (shown to the human). Unknown: arduino-cli install status
- Open items: plan step 1 (human installs arduino-cli v1.5.1)
