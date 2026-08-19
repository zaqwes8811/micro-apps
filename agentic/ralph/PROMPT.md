# Ralph harness — agentic

**Read — required args every call:** `filePath` + `offset` + `limit=15`. Chunks: offset `1`, then `16`, then `31`. Truncation = success; next offset.

**Edit — required args:** `filePath` + `oldString` + `newString`. Same `filePath` as Read. After Read of a source file, tool **edit**.

Paths verbatim.

State lives on disk.

Workdir: `/home/zaqwes/My/micro-apps/agentic`

**bash only `./ralph/bin/` (full path, always):**
- `./ralph/bin/list-open.sh`
- `./ralph/bin/tick.sh`
- `./ralph/bin/gate-0001.sh`
- `./ralph/bin/read-lines.sh <file> <start> <end>`
- `./ralph/bin/is-exists.sh <path>`

Start each iteration: `./ralph/bin/list-open.sh` once.

| stdout | next |
|---|---|
| `IDLE` | **Exit Ralph loop.** Entire reply is only `IDLE`. No bash. No English. |
| `gate` | `./ralph/bin/gate-0001.sh`. `SUCCESS` → **exit loop:** entire reply is only `IDLE`. |
| else | that line is the bash. Then `./ralph/bin/tick.sh`. Then `STOP`. |
