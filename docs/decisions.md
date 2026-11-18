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

## Question template (subagent -> Lead)
- **Question:**
- **Why it blocks:**
- **Options:** A / B / C
- **My recommendation and reason:**
- **Who else might know:** (repo file / other agent / human)

## Handoff template (agent -> Lead)
Done / Files changed / Interfaces changed / Verified / Assumed / Unknown / Questions / New libraries
