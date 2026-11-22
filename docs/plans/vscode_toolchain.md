# Plan: VS Code toolchain (arduino-cli)

- Status: approved by the human 2026-11-19
- Decisions: 21-27 in `docs/decisions.md`
- Branch: `chore/vscode-toolchain` from `main` (a14f0a8, Stage 0 merged)
- Working folder: `C:\Users\s0533\OneDrive\Arduino\cooking_temperature_control` (decision 29, amends 24)

## Steps

| # | Step | Owner |
|---|---|---|
| 1 | Install arduino-cli (no clone: the repo stays in OneDrive, decision 29) | human |
| 2 | Create branch `chore/vscode-toolchain` from `main` | git-release |
| 3 | Update `AGENTS.md`, `CLAUDE.md`, `.claude/settings.json`, the agent files and `.claude/agents/build-runner.md` (the Lead drafts, the human approves and applies) | human |
| 4 | Copy `tft_setup/Setup206_LilyGo_T_Display_S3.h` into the repo, word for word | hardware-pins |
| 5 | `sketch.yaml`, `tools/build.*`, `.vscode/tasks.json` | build-runner |
| 6 | First compile: exit code 0, binary size similar to the Arduino IDE build | build-runner |
| 7 | Review | qa-safety |
| 8 | Upload with the human's approval; check that Serial works | human, then build-runner |
| 9 | Update docs (`dependencies.md` with the real versions), commit and PR | Lead, git-release |

## Files to update
`AGENTS.md`, `CLAUDE.md`, `.claude/agents/qa-safety.md`, `.claude/agents/git-release.md`, `.claude/agents/build-runner.md` (new), `.claude/settings.json`, `docs/decisions.md`, `docs/dependencies.md`, `docs/open_questions.md`, `docs/test_plan.md`, `docs/stage0_refactor.md`, `docs/hardware.md`, `README.md`.

## Constraints from the decisions
- Core pinned to 3.3.11 (21). No PlatformIO / pioarduino.
- Board options exactly as in decision 22. Step 5 maps each menu item to its FQBN option key with `arduino-cli board details`; any key not covered by decision 22 goes to the human, never guessed.
- No 16MB / OPI change before Stage 3 (23).
- Every upload needs the human's approval (26).
- "Compiled" only with exit code 0 + command + first 30 error lines verbatim (27).

## Resolved points
- Docs-only commits without a compile output are allowed (decision 28).
- Working folder stays in OneDrive (decision 29).

## Progress
- Step 2 done 2026-11-19: branch `chore/vscode-toolchain` from a14f0a8.
- Step 1 done 2026-11-19: arduino-cli 1.5.1 at `C:\tools\arduino-cli` (human).
- Step 3 done 2026-11-19: drafts approved and applied (decision 30).
- Step 4 done 2026-11-19: tft_setup copy byte-identical (cmp).
- Step 5 done 2026-11-19: build files written (decisions 31-34).
- Step 6 2026-11-19: compiled, exit code 0, 365286 bytes (27%); IDE size comparison pending.
- Step 7 done 2026-11-19: qa-safety APPROVED FOR COMPILE AND BENCH TEST, 9 findings (fix bundle A-D below, pending).

## Where we stopped (end of 2026-11-19) — resume here
Branch `chore/vscode-toolchain`, last commit ef7525c (docs only). Everything after it is UNCOMMITTED in the working tree:
AGENTS.md, CLAUDE.md, .claude/settings.json, .claude/agents/{qa-safety,git-release,build-runner}.md, cooking_temperature_control/sketch.yaml, tools/build.ps1, tools/upload.ps1, .vscode/tasks.json, tft_setup/Setup206_LilyGo_T_Display_S3.h, docs/{decisions,open_questions,worklog,test_plan}.md, this file.
They cannot be committed as docs-only (they include build files and a `.h`): commit in step 9 after step 8.
Also uncommitted (side task, decision 35): all sources moved (git rename, staged) from the repo root to `cooking_temperature_control/`; README.md, docs/architecture.md updated to the new path. Compiled from the new folder, exit code 0, same sizes (365286 / 28360); new binary 365440 bytes, built 2026-11-19 19:41. The human chose to commit + push + PR only after the step 8 bench test (no exception to CLAUDE.md step 11).

### Next, in order
- 2026-11-22: step 8 DONE (upload COM13 exit 0; bench test accepted, decision 36). Step 9: commit + push + PR. Fix bundle A-D and F2-F4 stay open.
1. **Step 8 — upload (human approval each time):** human connects the board (heater and relay disconnected), closes every Serial Monitor, says "ready" -> build-runner runs `arduino-cli board list` -> Lead asks approval naming port + `C:\Users\s0533\AppData\Local\arduino-build\cooking_temperature_control\cooking_temperature_control.ino.bin` (365440 bytes, built 2026-11-19 19:41 from the new sketch folder; rebuild first if any source changed) -> build-runner runs `tools/upload.ps1 -Port COMxx -Confirmed` -> human runs the checklist in `docs/test_plan.md` ("Toolchain" section).
2. **Fix bundle A-D (human approval pending; human-owned files):**
   - A. `.claude/settings.json`: leading-`*` forms of all arduino-cli rules (`Bash(*arduino-cli*lib install*)`, `Bash(*arduino-cli*compile*-u*)`, ...), deny `board attach`, the same rules as `PowerShell(...)`, git denies `Bash(git push *main*)`, `*master*`, `*--force*`, `*+*`.
   - B. `.claude/settings.local.json`: remove `Bash(git push *)` allow.
   - C. `git-release.md`: docs-only check also stops on `sketch.yaml`, `tools/`, `.vscode/`.
   - D. Wording: core version is enforced in build.ps1 (not sketch.yaml) in AGENTS.md + build-runner.md; Setup206 has a reference copy in `tft_setup/`; build-runner reads decisions 21-34; upload approval names the .bin.
3. **Human inputs still missing:** Arduino IDE size lines for the same commit (step 6 comparison); keep build.ps1 without `-v` (Lead recommends yes).
4. **Step 9:** dependencies.md (arduino-cli 1.5.1, versions), test_plan.md line 3 and stage0_refactor.md line 87 ("the human compiles"), README build section, hardware.md if needed; then build-runner compile report + git-release commit and PR (no push to main).
