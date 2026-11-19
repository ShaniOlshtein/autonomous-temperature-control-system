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
