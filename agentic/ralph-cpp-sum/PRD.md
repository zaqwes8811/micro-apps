# Ralph Wiggum Loop - Iteration 2

You are in an iterative development loop. Work on the task below until you can genuinely complete it.

## Your Task

# Ralph harness — C++ ralph-cpp-sum

**Workdir (sandbox):** `/home/zaqwes/My/micro-apps/agentic/ralph-cpp-sum`

Stay **inside this directory only**. Do not leave it for any reason.

## Current task

1. Read **`PRD.md`** — scope, requirements, acceptance criteria for the current feature.
2. Read **`progress.md`** — checklist and iteration log; update checklist each iteration.
3. **Do not regress** existing work (`ralph_sum.hpp`, `test_ralph_sum.cpp`, bool-rejection in `CMakeLists.txt`).

## Sandbox

- All file reads/writes and shell commands must stay under the workdir above.
- Do **not** `cd ..`, `cd` to `agentic/`, home, `/tmp`, or any path outside the workdir.
- Do **not** read, edit, or create files in sibling folders (`../small`, `../to-micro-apps`, etc.).
- Use **relative paths** from the workdir (`./csv_reader.hpp`, `./build/`).
- Bash: run builds/tests from `.` — never from parent repos.

## Build & verify

**FORBIDDEN (OpenCode denies + no binary at `./build/*_test`):** `./build/csv_reader_test`, `./build/ralph_sum_test`, `ctest`, any path under `build/.internal/`. Running them hangs without timeout and kills llama-server.

After each meaningful change, run **one** bash call from project root:

```bash
./verify.sh
```

This is the **only** allowed way to run tests. Do not run bare `ctest` or any test binary.

**Test duration (enforced in harness, not optional):**

- Each test runs under **`timeout 5`** (wired in `CMakeLists.txt`).
- Whole verify capped at **15s** (`verify.sh`).
- If verify **exceeds ~15s** or a test shows **Timeout / exit 124** → **infinite loop in the code**. Fix implementation, rebuild, re-run `./verify.sh`. Do not retry unchanged.

### Required green output (all three layers)

**Layer A — compile-time (bool rejection)** — must appear in `./verify.sh` stdout during configure:

```
Bool rejection test PASSED: sum and sub(bool, ...) correctly rejected at compile time
```

**Layer B — runtime (math)** — in ctest section:

```
Start 1: ralph_sum_test
...
Passed
```

**Layer C — runtime (csv)** — in ctest section:

```
Start 2: csv_reader_test
...
Passed
```

**Layer D — ctest summary** — exact requirement:

```
100% tests passed, 0 tests failed out of 2
```

**Layer E — exit code:** `./verify.sh` must exit **0**. Exit **8**, **124**, or non-zero = **NOT DONE**.

If any layer is missing, failed, timed out, or shows `***Failed` / `Subprocess killed` → **keep working**. Do not emit COMPLETE.

Expected when green:

```
Bool rejection test PASSED: sum and sub(bool, ...) correctly rejected at compile time
...
Start 1: ralph_sum_test
1/2 Test #1: ralph_sum_test ...................   Passed
Start 2: csv_reader_test
2/2 Test #2: csv_reader_test ..................   Passed
100% tests passed, 0 tests failed out of 2
```

**Wrong:**

```bash
ctest --test-dir build --output-on-failure    # ❌ bypasses verify.sh hard cap
./build/csv_reader_test                       # ❌ wrong cwd; no timeout wrapper
cd build && ctest                             # ❌ wrong cwd
\"../test_data/sample.csv\" in test code        # ❌ use test_data/sample.csv
```

Do not run git commands (disabled in OpenCode); the human commits from the parent repo.

## Agent execution rules (critical on 4GB VRAM / turbo3)

**Act, don't narrate.** Long text without tool calls causes timeouts (exit 137) and can crash llama-server.

| Rule | Do | Don't |
|------|----|------|
| After edit | **Immediately** run `./verify.sh` | Narrate plans without a tool call |
| Verify | Always `./verify.sh` from project root | Bare `ctest` or direct test binaries |
| Replies | ≤ 2 short sentences between tool calls | Long explanations before verify |
| On test fail / timeout | Fix code → `./verify.sh` again | Retry same command unchanged |
| verify > 15s or exit 124 | **Infinite loop** — fix code | Wait, blame environment |
| Before COMPLETE | `./verify.sh` in **this turn** with exit 0 | Promise from memory or prior iteration |

After a code edit, your **very next action** must be `./verify.sh` — **same turn**, no intermediate text-only reply.

If llama-server died (tab closed): **stop** — human must restart `./rtx3050-4g/llamacpp-serve-qwen35-4b-turbo3.sh` before you continue.

## Checklist

Update **`progress.md`** each iteration. Do not duplicate the checklist here.

## STOP — COMPLETE gate (strict)

Emit `<promise>COMPLETE</promise>` **only** when **every** gate below is true **in the same iteration**:

| # | Gate | Required |
|---|------|----------|
| 1 | Tool call | You ran `./verify.sh` via bash **in this turn** (not a previous iteration) |
| 2 | Exit code | Tool output shows verify exited **0** |
| 3 | Compile-time | Output contains `Bool rejection test PASSED` |
| 4 | Runtime | Output contains `100% tests passed, 0 tests failed out of 2` |
| 5 | Both suites | Both `ralph_sum_test` and `csv_reader_test` show **Passed** (not Failed / Timeout / Subprocess killed) |
| 6 | No regression | No `FATAL_ERROR`, no `BOOL_*_COMPILES` configure failure |

### Forbidden COMPLETE (instant reject)

Do **not** emit COMPLETE if **any** of these is true:

- No bash tool call in this turn
- `Tools: none` and you only output text
- You did not run `./verify.sh` in this iteration
- Last `./verify.sh` failed, timed out, or exited non-zero
- Output shows `50% tests passed`, `***Failed`, `Subprocess killed`, or test duration ~5s with failure (timeout = infinite loop — fix code first)
- Only compile-time passed but runtime csv failed (or vice versa)
- You \"believe\" tests pass without fresh tool output in this turn
- Checklist in `progress.md` still has unchecked items

### How to finish (only valid sequence)

1. Fix code until `./verify.sh` is green (all layers A–E above).
2. Run `./verify.sh` — **same turn**, last tool call before your reply.
3. Reply: **one short paragraph** quoting verify exit 0 + `100% tests passed, 0 tests failed out of 2` + `Bool rejection test PASSED`.
4. Last line only: `<promise>COMPLETE</promise>`

**Invalid:** reply with COMPLETE without step 2 in the same turn.

<promise>COMPLETE</promise>

The line above is a **format example only**. Output it **once**, on its own line, **only** after a green `./verify.sh` in the same turn. Never copy this example as completion without verify output.


## Instructions

1. Read the current state of files to understand what's been done
2. Track your progress and plan remaining work
3. Make progress on the task
4. Run tests/verification if applicable
5. When the task is GENUINELY COMPLETE, output:
   <promise>COMPLETE</promise>

## Critical Rules

- ONLY output <promise>COMPLETE</promise> when the task is truly done
- Output the promise tag DIRECTLY - do not quote it, explain it, or say you \"will\" output it
- Do NOT lie or output false promises to exit the loop
- If stuck, try a different approach
- Check your work before claiming completion
- The loop will continue until you succeed

## Current Iteration: 2 / 25 (min: 1)

Now, work on the task. Good luck!