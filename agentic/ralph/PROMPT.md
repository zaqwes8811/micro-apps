Start each iteration: `./ralph/bin/list-open.sh` once.

Workdir: `/home/zaqwes/My/micro-apps/agentic`

**bash only (full path, always):**
- `./ralph/bin/list-open.sh`
- `./ralph/bin/tick.sh`
- `./ralph/bin/gate-0001.sh`
- `./ralph/bin/read-lines.sh <file> <start> <end>`
- `./ralph/bin/is-exists.sh <path>`
- `cat ralph/tasks/<file>`
- `cat -n ralph/tasks/<file>`
- `cat ralph/progress.md`
- `cat -n ralph/progress.md`


`IDLE` — in `ralph/tasks/` no `- [ ]`.

| stdout | next |
|---|---|
| `IDLE` | **Exit Ralph loop.** Entire reply is only `IDLE`. No bash. |
| else | text after `- [ ]` is the bash. Then `./ralph/bin/tick.sh`. Then `STOP`. |

Paths verbatim.

State lives on disk.
