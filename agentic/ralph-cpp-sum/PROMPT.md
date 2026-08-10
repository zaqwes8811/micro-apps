# Ralph harness — C++ ralph-cpp-sum

**Workdir (sandbox):** `/home/zaqwes/My/micro-apps/agentic/ralph-cpp-sum`

Stay **inside this directory only**. Do not leave it for any reason.

## Fast exit (read progress.md first)

If **every** Status line in `progress.md` is `[x]` (zero `[ ]`):

1. **One** bash tool call: `./verify.sh`
2. If green (`100% tests passed, 0 tests failed out of 2`, exit 0): your **next** message must be **only** the completion promise tag from `PRD.md` — **nothing else**. No criteria list, no summary, no progress edit, no second verify, no `echo $?`.
3. If verify fails: fix code → `./verify.sh` once → then step 2.

**Do not** edit `progress.md` when all items are already `[x]`. **Do not** run `./verify.sh` more than once when output is already green.

## Current task

1. Read **`PRD.md`** — scope, requirements, acceptance criteria.
2. Read **`progress.md`** — if any `[ ]`, do that item; if all `[x]`, use **Fast exit** above.
3. **Do not regress** sum/sub + bool-rejection.

## Sandbox

- All reads/writes/shell under this workdir only.
- No `cd ..`, `/tmp`, sibling folders.
- Relative paths from `.` (`test_data/sample.csv`).

## Build & verify

**FORBIDDEN:** `./build/*_test`, `ctest`, `build/.internal/*` — use **only** `./verify.sh`.

```bash
./verify.sh
```

- Per-test cap **5 s** (CMake); whole run cap **15 s** (`verify.sh`).
- Exit **124** or run >15 s → infinite loop in code; fix, then verify once.

Green output must include: `Bool rejection test PASSED`, both tests **Passed**, `100% tests passed, 0 tests failed out of 2`, exit **0**.

## Agent rules (4GB VRAM / turbo3)

| Rule | Do | Don't |
|------|----|-------|
| Already done | Fast exit: 1× verify → promise only | Re-verify, edit progress, narrate criteria |
| After code edit | `./verify.sh` immediately | Text-only reply before verify |
| Replies | ≤2 short sentences between tools | Long acceptance-criteria lists |
| Complete | Promise tag **alone** on last line | Paragraph before promise |

Do not run git. If llama tab died → stop (human restarts server).

## Checklist

When work remains: update **`progress.md`** (use **edit**, not write, for small log lines). When all `[x]`: **do not touch** progress.md.

## COMPLETE gate

Emit the completion promise tag from **`PRD.md`** only when:

- You ran `./verify.sh` **in this turn** and output is green, **or**
- Fast exit applies (all `[x]` + one green verify).

**Forbidden:** promise without green verify in this turn; promise plus extra text (summary, checklist, tool calls after verify); running verify twice when first run was green.

The promise tag is defined in `PRD.md` — copy it exactly on its own line with no prefix or suffix.
