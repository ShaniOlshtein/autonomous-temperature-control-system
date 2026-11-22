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

## 2026-11-19 — Toolchain plan step 3: drafts
- Requested by: human (arduino-cli 1.5.1 installed; "continue")
- Delegated to: none (the Lead drafts human-owned files; the human approves and applies)
- Files changed: docs/plans/drafts/step3/ (AGENTS.md, CLAUDE.md, .claude/settings.json, .claude/agents/qa-safety.md, git-release.md, build-runner.md), docs/worklog.md
- Result: drafts written; settings.json rules checked against code.claude.com/docs/en/permissions (deny > ask > allow; ask rules prompt even in auto mode; Bash rules are not a security boundary)
- Verified / Assumed / Unknown: Verified: arduino-cli 1.5.1 on PATH (`/c/tools/arduino-cli`), config empty (defaults), core 3.3.11 present in %LOCALAPPDATA%\Arduino15 (shared with Arduino IDE 1.8.19); IDE sketchbook is `C:\Users\s0533\OneDrive\מסמכים\Arduino` (Hebrew path); IDE last port COM13 (decision 22 says COM11). Unknown: whether arduino-cli finds the IDE's libraries without config
- Open items: human approval of the drafts; build-runner has no Write tool but owns files; COM11 vs COM13; library folder for arduino-cli

## 2026-11-19 — Toolchain plan steps 3 and 4
- Requested by: human (drafts approved; three recommendations accepted)
- Delegated to: hardware-pins (step 4: verbatim copy of the TFT_eSPI setup)
- Files changed: AGENTS.md, CLAUDE.md, .claude/settings.json, .claude/agents/qa-safety.md, .claude/agents/git-release.md, .claude/agents/build-runner.md (new) (approved drafts applied by the Lead); tft_setup/Setup206_LilyGo_T_Display_S3.h (new, hardware-pins); docs/decisions.md (30-33); docs/open_questions.md (Q18 removed); docs/plans/vscode_toolchain.md
- Result: step 3 applied; step 4 copy is byte-identical to the source (`cmp`). Active include in User_Setup_Select.h is Setup206; it defines `TFT_BL 38`, same pin as `LCD_BL` in pins.h (decision 11)
- Verified / Assumed / Unknown: Verified: `cmp` identical; TFT_eSPI library lives in `C:\Users\s0533\OneDrive\Arduino\libraries\arduino_853159` (ASCII path), while arduino-cli `directories.user` defaults to `C:\Users\s0533\OneDrive\מסמכים\Arduino`. Nothing compiled
- Open items: step 5 (build-runner); hardware-pins offer to cite tft_setup/ in pins.h and hardware.md (not requested, not done)

## 2026-11-19 — Toolchain plan step 5: build-runner proposal
- Requested by: human (continue the plan)
- Delegated to: build-runner (read-only survey; proposed sketch.yaml, tools/build.ps1, tools/upload.ps1, .vscode/tasks.json; nothing written, nothing compiled)
- Files changed: docs/worklog.md
- Result: all 17 board options of core 3.3.11 map 1:1 to decision 22 (no uncovered option). No sketch.yaml profile (a profile would download the core and libraries); `default_fqbn` + a 3.3.11 guard in build.ps1. Libraries are found only with ARDUINO_DIRECTORIES_USER=C:\Users\s0533\OneDrive\Arduino (process-only; config file unchanged). Setup206 is used from the installed library; build.ps1 checks it equals tft_setup/
- Verified / Assumed / Unknown: Verified (command output): options mapping, libraries TFT_eSPI 2.5.43, OneWire 2.3.8, DallasTemperature 4.0.6 found with the override; core 3.3.11 installed (3.3.12 available, not to be installed). Side effect: the first `arduino-cli core list` downloaded arduino-cli built-in tools (dfu-discovery 0.1.2, mdns-discovery 1.1.0, serial-discovery 1.5.2, serial-monitor 0.15.0, ctags 5.8-arduino12) into %LOCALAPPDATA%\Arduino15; not a core or library. Assumed: profile behavior from the docs. Unknown: PowerShell version and execution policy
- Open items: human decisions on profile vs default_fqbn, upload script/task, --warnings all

## 2026-11-19 — Toolchain plan steps 5 and 6: build files and first compile
- Requested by: human (Q1-Q3 approved with additions, decision 34)
- Delegated to: build-runner (proposed files; revised upload.ps1/tasks.json; first compile), Lead wrote the files unchanged (decision 31)
- Files changed: sketch.yaml, tools/build.ps1, tools/upload.ps1, .vscode/tasks.json (new); docs/decisions.md (34); docs/worklog.md
- Result: Compiled, exit code 0. "Sketch uses 365286 bytes (27%) of program storage space. Maximum is 1310720 bytes." "Global variables use 28360 bytes (8%) of dynamic memory ... Maximum is 327680 bytes." No project-file warnings (8 library/core warnings). Binary C:\Users\s0533\AppData\Local\arduino-build\cooking_temperature_control\cooking_temperature_control.ino.bin, 365440 bytes, 2026-11-19 18:06
- Verified / Assumed / Unknown: Verified (build-runner output): exit code 0, guards passed, sizes, PowerShell 5.1.26100.9711. Unknown: why no "Used library" table was printed; IDE binary size for comparison (human)
- Open items: IDE size comparison; qa-safety review (step 7); upload with approval (step 8)

## 2026-11-19 — Toolchain plan step 7: qa-safety review
- Requested by: human (plan)
- Delegated to: qa-safety (APPROVED FOR COMPILE AND BENCH TEST, 9 findings: 3 medium, 6 low/info)
- Files changed: docs/test_plan.md (toolchain upload checklist), docs/worklog.md
- Result: build files match decisions 22, 26, 32, 34 by reading (FQBN checked against the installed boards.txt). Findings in human-owned files: settings.json rules match only commands starting with `arduino-cli` (full path bypasses), no PowerShell(...) rules, settings.local.json allows `git push *`, git-release docs-only check tests extensions only; text inconsistencies (core version is enforced in build.ps1, not sketch.yaml; Setup206 "outside this repo")
- Verified / Assumed / Unknown: Verified by the Lead against code.claude.com/docs/en/permissions: `*` may appear anywhere in a rule; PowerShell rules use the same shape. Verified: arduino-cli is on PATH in the Lead's Bash (/c/tools/arduino-cli). Unknown: GitHub branch protection; IDE size parity
- Open items: human decision on the fix bundle; upload approval (step 8)

## 2026-11-19 — End of session: status recorded
- Requested by: human ("update everything so we know where we are; tomorrow continue with the upload")
- Delegated to: none
- Files changed: docs/plans/vscode_toolchain.md ("Where we stopped" section), docs/open_questions.md (section F), docs/worklog.md
- Result: plan steps 1-7 done; step 8 (upload) is next; fix bundle A-D and IDE size pending; all work after ef7525c is uncommitted on `chore/vscode-toolchain`
- Verified / Assumed / Unknown: Verified: compile exit code 0 (build-runner); nothing uploaded; nothing tested on hardware with the arduino-cli build
- Open items: see `docs/plans/vscode_toolchain.md` "Next, in order" and `docs/open_questions.md` F1-F4

## 2026-11-19 — Sources moved to the sketch folder `cooking_temperature_control/`
- Requested by: human (side task; pre-approved without per-step approvals)
- Delegated to: build-runner (compile only); Lead updated the path references
- Files changed: sources already moved (git rename, staged) from the repo root to `cooking_temperature_control/` (12 files + sketch.yaml); AGENTS.md (Where things are, new "Source:" line, ownership table), .claude/agents/build-runner.md, .claude/agents/qa-safety.md, .vscode/tasks.json (problemMatcher fileLocation), README.md (IDE/CLI paths), docs/architecture.md, docs/plans/vscode_toolchain.md, docs/decisions.md (35), docs/worklog.md. tools/build.ps1 and tools/upload.ps1 already used the folder (unchanged)
- Result: Compiled, exit code 0 from `...\cooking_temperature_control\cooking_temperature_control`. "Sketch uses 365286 bytes (27%) of program storage space. Maximum is 1310720 bytes." "Global variables use 28360 bytes (8%) of dynamic memory ... Maximum is 327680 bytes." Same as before the move. No project-file warnings. No .ino/.cpp/.h left in the repo root. Binary .ino.bin 365440 bytes, 2026-11-19 19:41
- Verified / Assumed / Unknown: Verified (build-runner output): exit code 0, sizes, core 3.3.11, TFT_eSPI 2.5.43, OneWire 2.3.8, DallasTemperature 4.0.6. Historical docs (decisions 1-34, stage0_refactor.md, old worklog entries) keep bare file names on purpose. Unknown: nothing tested on hardware
- Open items: none new; nothing committed (all work stays uncommitted on `chore/vscode-toolchain`)

## 2026-11-22 — Toolchain plan step 8: first arduino-cli upload
- Requested by: human (approved upload to COM13 of the 2026-11-19 19:41 .bin; heater and relay disconnected, Serial Monitors closed)
- Delegated to: build-runner (board list: only COM13; upload via tools/upload.ps1 -Port COM13 -Confirmed)
- Files changed: docs/worklog.md
- Result: upload exit code 0; esptool v5.3.1 connected to ESP32-S3 (QFN56) rev v0.2, 8MB embedded PSRAM; bootloader, partitions, boot_app0 and app (365440 bytes at 0x10000) each "Hash of data verified."; hard reset via RTS. No sources newer than the .bin
- Verified / Assumed / Unknown: Verified (build-runner output): exit code 0, hashes verified. Unknown: board behavior after reset (human bench test pending). Note: human has only ONE DS18B20 connected this time (expected "1/2")
- Open items: human bench test (docs/test_plan.md "Toolchain"); then git-release commit + push + PR on chore/vscode-toolchain

## 2026-11-22 — Toolchain bench test result and SW3 diagnosis
- Requested by: human
- Delegated to: none (Lead read input.cpp; human measured ADC in Serial)
- Files changed: docs/decisions.md (36), docs/test_plan.md (results, intro line), docs/open_questions.md (section G), docs/dependencies.md (arduino-cli 1.5.1 CONFIRMED), docs/worklog.md
- Result: bench test passes except SW3 (ADC 1702-1705 falls just above the 1700 SW3 limit, decoded as SW4; SW4 reads 2789-2793) and SW5 (hardware fault, human fixes). Both existed before the toolchain change
- Verified / Assumed / Unknown: Verified: human-reported Serial lines. Unknown: SW1, SW2, SW5, no-button ADC values on this board
- Open items: commit/push/PR chore/vscode-toolchain; then fix/adkey-sw3-threshold (decision 36); F1-F4, G1-G2
