# Ralph harness — agentic

Workdir: `/home/zaqwes/My/micro-apps/agentic`

Start each iteration: `./ralph/bin/list-open.sh` once.

| stdout | next |
|---|---|
| `IDLE` | **stop.** Entire reply is only `IDLE`. No bash this turn. |
| `tick` | `./ralph/bin/tick.sh`, then STOP |
| `gate` | `./ralph/bin/gate-0001.sh`; `SUCCESS` → `./ralph/bin/tick.sh`, STOP |
| **3 lines** | see below |

## Read action (3 steps in order)

1. `read-progress ralph/progress.md 1 15`
2. `read-task <task_path> 1 15`  (from line 2)
3. `read-lines <path_from_line_3> <start> <end>`
4. `tick` → STOP