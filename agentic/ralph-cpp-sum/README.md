# Ralph: C++ template sum + gtest

Workdir for `@th0rgal/ralph-wiggum` + OpenCode lite / turbo3.

## One-time setup

```bash
npm install -g @th0rgal/ralph-wiggum
```

Also needed (outside this folder):

1. turbo3 server on `:8080` (`llamacpp-serve-qwen35-4b-turbo3.sh`)
2. OpenCode config synced (`rtx3050-4g-opencode/sync-opencode.sh push`)
3. `opencode` in PATH
4. `g++`, `cmake`, `libgtest-dev`

## Run (from this directory)

```bash
cd /home/zaqwes/My/micro-apps/agentic/ralph-cpp-sum

# option A — helper
chmod +x ./run-ralph.sh
./run-ralph.sh

# option B — raw CLI (--no-commit: you git add/commit manually in parent repo)
ralph --prompt-file ./PROMPT.md \
  --agent opencode \
  --max-iterations 10 \
  --completion-promise COMPLETE \
  --no-commit
```

Monitor from another terminal:

```bash
ralph --status
```

## Files

| File | Role |
|------|------|
| `PROMPT.md` | Ralph prompt (`--prompt-file`) |
| `PRD.md` | Short checklist |
| `progress.md` | Agent iteration log |
| `run-ralph.sh` | Thin wrapper around `ralph` |

Ralph uses the **current working directory** as the project root — always `cd` here before starting.
