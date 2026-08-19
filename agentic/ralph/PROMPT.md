# Ralph harness — agentic

Workdir: `/home/zaqwes/My/micro-apps/agentic`

Start each iteration: `./ralph/bin/list-open.sh` once.

| stdout | next |
|---|---|
| `idle` | **stop.** Entire reply is only `<promise>idle</promise>`. No read, no tick, no other bash. |
| `tick` | `./ralph/bin/tick.sh`, then STOP |
| `gate` | `./ralph/bin/gate-0001.sh`; `SUCCESS` → `./ralph/bin/tick.sh`, STOP |
| **3 lines** | see below |

**Three lines** (in order):

1. tool **read** line 1 (`ralph/progress.md`) — `offset=1`, `limit=15`
2. tool **read** line 2 (task path) — same
3. bash line 3 exactly as printed (e.g. `./ralph/bin/read-lines.sh config.yaml 1 10`). Then `./ralph/bin/tick.sh`, STOP
