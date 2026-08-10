# Ralph: C++ sum + sub + CSV reader + gtest

Workdir for `@th0rgal/ralph-wiggum` + OpenCode **ralph** agent / turbo3.

Current task: template **sum/sub**, **CsvReader** (CSV file parser), gtest + compile-time bool rejection.

## One-time setup

```bash
npm install -g @th0rgal/ralph-wiggum
```

**npm `@latest` = 1.2.2** — requires **Bun** in PATH; no `--last-activity-timeout` (only in unpublished GitHub 1.3.1). `run-ralph.sh` adds `~/.bun/bin` to PATH and skips unknown flags.

If `npm install` fails with **ENOTEMPTY**, clean first:

```bash
npm uninstall -g @th0rgal/ralph-wiggum
rm -rf "$(npm root -g)/@th0rgal/ralph-wiggum" "$(npm root -g)/@th0rgal/.ralph-wiggum-"*
npm install -g @th0rgal/ralph-wiggum@latest
ralph --help | head
```

Skip `npm install -g github:Th0rgal/open-ralph-wiggum` for now — npm git install is flaky; wait for 1.3.1 on npm.

Also needed (outside this folder):

1. turbo3 server on `:8080` (`llamacpp-serve-qwen35-4b-turbo3.sh`)
2. OpenCode config synced (`rtx3050-4g-opencode/sync-opencode.sh push`) — **timeout 600000** for llamacpp
3. `opencode` in PATH
4. `g++`, `cmake`, `libgtest-dev`

## Run (from this directory)

```bash
cd /home/zaqwes/My/micro-apps/agentic/ralph-cpp-sum

# option A — helper (default: OpenCode ralph agent; 25 iterations, 30 steps/iteration)
chmod +x ./run-ralph.sh
./run-ralph.sh

# more iterations for debugging
MAX_ITERATIONS=40 ./run-ralph.sh

# option B — lite agent instead of ralph
OPENCODE_AGENT=lite ./run-ralph.sh
# or
./run-ralph.sh -- --agent lite

# option C — raw CLI (add --last-activity-timeout 5m only if `ralph --help` lists it)
ralph --prompt-file ./PROMPT.md \
  --agent opencode \
  --max-iterations 25 \
  --completion-promise COMPLETE \
  --no-commit \
  -- --agent ralph
```

Monitor from another terminal:

```bash
ralph --status
```

## Troubleshooting

| Symptom | Likely cause | Fix |
|---------|--------------|-----|
| Exit **137**, llama tab closed | GPU/system OOM or process killed | Restart `llamacpp-serve-qwen35-4b-turbo3.sh`; check `dmesg \| grep -i oom` |
| Stuck after "Now let's run tests", **Tools: none** | Model narrates instead of bash | `./run-ralph.sh` + PROMPT rules; optional: ralph from GitHub for `--last-activity-timeout` |
| **MAXIMUM STEPS REACHED** mid-iteration | OpenCode `ralph` agent step cap (was 12) | `agent.ralph.steps: 30` in opencode.json; override: `MAX_ITERATIONS=40 ./run-ralph.sh` |
| Iteration ~3:00, no progress | Old OpenCode timeout 180s | Confirm `timeout: 600000` in `~/.config/opencode/opencode.json`, restart Ralph |
| ctest finds 0 tests | Wrong cwd | Run from **project root**: `ctest --test-dir build --output-on-failure` |
| CSV tests fail on paths | Tests run from `build/` | CMake must set `WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}`; paths `test_data/sample.csv` |

## Files

| File | Role |
|------|------|
| `PROMPT.md` | Ralph prompt (`--prompt-file`) |
| `PRD.md` | Short checklist |
| `progress.md` | Agent iteration log |
| `run-ralph.sh` | Wrapper: `--no-commit`, optional `--last-activity-timeout`, `--agent ralph` |
| `.ralph/` | Ralph loop state |

Ralph uses the **current working directory** as the project root — always `cd` here before starting.

Tests always run from **project root** (`ctest --test-dir build`), not from `build/`.
