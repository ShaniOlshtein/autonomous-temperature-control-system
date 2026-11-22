# Decisions log (the Lead maintains it; never re-ask a decided question)

| # | Date | Decision | Decided by |
|---|---|---|---|
| 1 | 2026-11-18 | The Lead is the main Claude Code session; the 7 subagents are specialists. Only the Lead talks to the human | human (accepted) |
| 2 | 2026-11-18 | The human gives one task at a time; the Lead proposes a plan and waits for approval before any branch or agent | human |
| 3 | 2026-11-18 | Agents never compile; the human compiles, uploads and bench-tests in her own Arduino environment | human |
| 4 | 2026-11-18 | Copilot is not used any more. After `AGENTS.md` is replaced, the Copilot agent, skill and `copilot-instructions.md` are deleted (git keeps history) | human |
| 5 | 2026-11-18 | **Stage 0**: split the existing sketch into modules first, with NO behavior change | human (accepted recommendation) |
| 6 | 2026-11-18 | `control-logic` owns the main `.ino` (setup/loop only call init/update) and `timer.*` | human (accepted recommendation) |
| 7 | 2026-11-18 | Range validation only in `config_store`; BLE checks only the wire format | human (accepted recommendation) |
| 8 | 2026-11-18 | Hard safety limits are compile-time constants in `safety.h`. **Values: UNKNOWN, the human must provide them** | human (accepted recommendation) |
| 9 | 2026-11-18 | Conflict rule lives in `config_store`: last write wins; local changes are notified to BLE | human (accepted recommendation) |
| 10 | 2026-11-18 | `qa-safety` is read-only; the Lead writes `docs/test_plan.md` from its report | human (accepted recommendation) |
| 11 | 2026-11-18 | `pins.h` keeps the existing macro names (`LCD_BL`, `ADKEY_OUT_PIN`, `TEMP_SENSOR_PIN`); display pins appear only as comments | human (accepted recommendation) |
| 12 | 2026-11-18 | `AGENTS.md` is the single source of rules | human (accepted recommendation) |
| 13 | 2026-11-18 | Timer **duration** (setting) is separate from **remaining time** (runtime state). Stage 0 keeps today's single value; the split comes in Stage 2 | human (accepted recommendation) |
| 14 | 2026-11-18 | Stage 0 works from `main` (now contains the 479-line sketch with SW5); branch `refactor/stage0-modules` is created from `main` | human |
| 15 | 2026-11-18 | Stage 0 keeps ALL current quirks unchanged, including SW5; each is listed in `docs/open_questions.md` section E to fix in Stage 1 | human |
| 16 | 2026-11-18 | `docs/for_humans/process_overview.md` was deleted on purpose; ignore it | human |
| 17 | 2026-11-18 | `docs/stage0_refactor.md` updated to the current sketch (SW5, background color, ADC debug print, their checks) | human (accepted recommendation) |
| 18 | 2026-11-18 | `cookingActive` and `uiNeedsRedraw` may be split across modules only if the redraw timing stays identical; otherwise keep them together and report | human |
| 19 | 2026-11-18 | Stage 0 contracts approved: `config_store.h`, `sensors.h`, `timer.h`, `display_ui.h`, `input.h` (25 public functions, module-prefix names); `pins.h` unchanged. Implementation order: config_store → sensors, timer → display_ui, input → `.ino`; qa-safety compares old vs new before the human compiles | human |
| 20 | 2026-11-19 | `git-release` runs on the `haiku` model (`model: haiku` in `.claude/agents/git-release.md`), intentionally | human |
| 21 | 2026-11-19 | Build tool: **arduino-cli** with the ESP32 core **pinned to 3.3.11**. Not PlatformIO, not pioarduino. Supersedes decision 3 ("agents never compile"); decision 3 stays in this table for history. Takes effect for agents once `AGENTS.md`/`CLAUDE.md` are updated (plan step 3, `docs/plans/vscode_toolchain.md`) | human |
| 22 | 2026-11-19 | Board settings are locked to exactly what the human verified in the Arduino IDE Tools menu: Board ESP32S3 Dev Module · Upload Speed 921600 · USB Mode Hardware CDC and JTAG · USB CDC On Boot Enabled · USB Firmware MSC On Boot Disabled · USB DFU On Boot Disabled · Upload Mode UART0 / Hardware CDC · CPU Frequency 240MHz (WiFi) · Flash Mode QIO 80MHz · Flash Size 4MB (32Mb) · Partition Scheme Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS) · Core Debug Level None · PSRAM Disabled · Arduino Runs On Core 1 · Events Run On Core 1 · Erase All Flash Before Sketch Upload Disabled · JTAG Adapter Disabled · Zigbee Mode Disabled · Port COM11. Any option the tool exposes that is not in this list: check it against the tool and ask the human; never guess | human |
| 23 | 2026-11-19 | Moving to 16MB flash / OPI PSRAM is NOT planned. Decide it only in Stage 3 (BLE), based on the program-size percentage reported by the compile | human |
| 24 | 2026-11-19 | Working folder: `C:\dev\cooking_temperature_control`, outside OneDrive (Hebrew path + bug #1239). The Lead verifies it is a valid clone, on `main`, with the correct remote; if not, it reports to the human and does not fix it alone | human |
| 25 | 2026-11-19 | Eighth agent **build-runner**: tools Read, Grep, Glob, Bash; no Edit/Write on code. Owns `sketch.yaml`, `tools/build.*`, `.vscode/tasks.json` | human |
| 26 | 2026-11-19 | Every upload (flash) needs the human's approval each time. The `settings.json` syntax is verified against the official documentation before it is applied | human |
| 27 | 2026-11-19 | Reporting rule: "Compiled" only when the exit code is 0, quoting the command and the first 30 error lines verbatim. "Tested" only after the human tests on the device. `qa-safety` stays read-only. `git-release` does not commit without a successful compile output | human |
| 28 | 2026-11-19 | Exception to decision 27 (27 stays): `git-release` may commit a **docs-only** change without a compile output, only if the change contains no `.ino`, `.h` or `.cpp` file (including `pins.h`). The Lead shows the human `git diff --stat` before such a commit | human |
| 29 | 2026-11-19 | Amends decision 24 (24 stays for history): the working folder stays `C:\Users\s0533\OneDrive\Arduino\cooking_temperature_control` (the only copy of the repo). No move to `C:\dev` | human |
| 30 | 2026-11-19 | Plan step 3 drafts approved and applied: `AGENTS.md`, `CLAUDE.md`, `.claude/settings.json`, `.claude/agents/qa-safety.md`, `git-release.md`, `build-runner.md` (new) | human |
| 31 | 2026-11-19 | `build-runner` has no Edit/Write (decision 25 stands): for its files it returns the full content and the Lead writes it unchanged; it never writes files through Bash | human (accepted recommendation) |
| 32 | 2026-11-19 | Amends decision 22 (port only): the upload port is not fixed in `sketch.yaml` or scripts. Before each upload `build-runner` shows `arduino-cli board list` and the human confirms the port (COM11 in decision 22, COM13 last used by the IDE) | human (accepted recommendation) |
| 33 | 2026-11-19 | Library location for arduino-cli is checked in plan step 5 by `build-runner` with read-only `arduino-cli lib list`; decide only if libraries are not found | human (accepted recommendation) |
| 34 | 2026-11-19 | Build files approved: no sketch.yaml profile (`default_fqbn` only; `tools/build.ps1` stops unless the core is 3.3.11); `tools/upload.ps1` + a human-run VS Code Upload task; `--warnings all`. Additions: every upload approval shows the binary file name and the port; the upload script reminds to close the Serial Monitor; compile reports list only warnings from project files | human |
| 35 | 2026-11-19 | Firmware sources (`.ino`, `.cpp`, `.h`) and `sketch.yaml` live in the sketch folder `cooking_temperature_control/`, not in the repo root; new modules go there too. Docs/agent files updated to the new path; the human pre-approved this task without per-step approvals | human |
| 36 | 2026-11-22 | Toolchain bench test accepted (SW3 and SW5 faults are pre-existing): commit, push and PR `chore/vscode-toolchain`. Then a separate branch `fix/adkey-sw3-threshold` (stacked on `chore/vscode-toolchain`, because the sources moved there): SW3/SW4 boundary in `input.cpp` changes from 1700 to 2250 (midpoint of measured SW3 1705 and SW4 2790), then compile, review, approved upload, bench test, commit, push. SW5 is a hardware fault the human fixes herself | human |

## Question template (subagent -> Lead)
- **Question:**
- **Why it blocks:**
- **Options:** A / B / C
- **My recommendation and reason:**
- **Who else might know:** (repo file / other agent / human)

## Handoff template (agent -> Lead)
Done / Files changed / Interfaces changed / Verified / Assumed / Unknown / Questions / New libraries
