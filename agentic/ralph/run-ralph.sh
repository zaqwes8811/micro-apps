#!/usr/bin/env bash
# Ralph loop cwd = agentic/ (harness = agentic/ralph/). OpenCode agent = ralph-4b.
set -euo pipefail

HARNESS="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKDIR="$(cd "$HARNESS/.." && pwd)"
cd "$WORKDIR"

export PATH="${BUN_INSTALL:-$HOME/.bun}/bin:${PATH}"

MAX_ITERATIONS="${MAX_ITERATIONS:-25}"
AGENT="${AGENT:-opencode}"
OPENCODE_AGENT="${OPENCODE_AGENT:-ralph-4b}"
LAST_ACTIVITY_TIMEOUT="${LAST_ACTIVITY_TIMEOUT:-5m}"
PROMPT_FILE="${PROMPT_FILE:-$HARNESS/PROMPT.md}"

if ! command -v ralph >/dev/null 2>&1; then
  echo "ralph not found. Install: npm install -g @th0rgal/ralph-wiggum"
  exit 1
fi
if ! command -v bun >/dev/null 2>&1; then
  echo "bun not found. Install: https://bun.sh"
  exit 1
fi
if ! command -v opencode >/dev/null 2>&1; then
  echo "opencode not found in PATH"
  exit 1
fi

export REAL_OPENCODE="$(command -v opencode)"
export RALPH_OPENCODE_BINARY="$HARNESS/bin/opencode"
export RALPH_RUNTIME_CONFIG="$HARNESS/.opencode.runtime.json"
chmod +x "$RALPH_OPENCODE_BINARY"

# Ralph 1.2.2 hashes every `git ls-files` path in cwd before spawning OpenCode.
# Point Ralph git at the tiny harness repo so it does not walk the workdir tree.
if [[ ! -d "$HARNESS/.git" ]]; then
  git -C "$HARNESS" init -q
fi
export GIT_DIR="$HARNESS/.git"
export GIT_WORK_TREE="$HARNESS"

python3 - "${HOME}/.config/opencode/opencode.json" "$HARNESS/opencode.json" "$RALPH_RUNTIME_CONFIG" <<'PY'
import json, re, sys
from pathlib import Path

def load(p):
    path = Path(p)
    return json.loads(path.read_text(encoding="utf-8")) if path.is_file() else {}

def rewrite(obj, base: Path):
    if isinstance(obj, dict):
        return {k: rewrite(v, base) for k, v in obj.items()}
    if isinstance(obj, list):
        return [rewrite(v, base) for v in obj]
    if isinstance(obj, str):
        m = re.fullmatch(r"\{file:(\./[^}]+)\}", obj)
        if m:
            return "{file:" + str(base / m.group(1)[2:]) + "}"
        return obj
    return obj

def merge(base, extra):
    """Deep merge so the harness can override one nested key (e.g.
    permission.bash) without dropping the rest of the user config."""
    for k, v in extra.items():
        if isinstance(v, dict) and isinstance(base.get(k), dict):
            merge(base[k], v)
        else:
            base[k] = v
    return base

user, extra, out = sys.argv[1], sys.argv[2], sys.argv[3]
user_path = Path(user)
cfg = rewrite(load(user), user_path.parent)
merge(cfg, load(extra))
cfg["snapshot"] = False
cfg["lsp"] = False
cfg["watcher"] = {"ignore": ["**/*"]}
Path(out).write_text(json.dumps(cfg, indent=2) + "\n", encoding="utf-8")
PY

RALPH_HELP="$(ralph --help 2>&1 || true)"
TIMEOUT_ARGS=()
if [[ "$RALPH_HELP" == *"--last-activity-timeout"* ]]; then
  TIMEOUT_ARGS=(--last-activity-timeout "$LAST_ACTIVITY_TIMEOUT")
fi

if [[ $# -ge 1 && "$1" == "--" ]]; then
  OPENCODE_ARGS=("$@")
else
  OPENCODE_ARGS=(-- --agent "$OPENCODE_AGENT" "$@")
fi

echo "cwd: $WORKDIR"
echo "harness: $HARNESS"
echo "ralph agent: $AGENT"
echo "opencode agent: $OPENCODE_AGENT"
echo "OPENCODE_CONFIG: $RALPH_RUNTIME_CONFIG (snapshot=false)"
echo "opencode via: $RALPH_OPENCODE_BINARY -> $REAL_OPENCODE"
echo "max-iterations: $MAX_ITERATIONS"
echo "Ralph git: $GIT_WORK_TREE (not workdir ls-files)"
echo "prompt: $PROMPT_FILE"
echo

if [[ "$(./ralph/bin/list-open.sh 2>/dev/null || echo STOP)" == "IDLE" ]]; then
  echo "Completion: IDLE — no open tasks."
  exit 0
fi

exec ralph \
  --prompt-file "$PROMPT_FILE" \
  --agent "$AGENT" \
  --max-iterations "$MAX_ITERATIONS" \
  --completion-promise "IDLE" \
  --no-commit \
  "${TIMEOUT_ARGS[@]}" \
  "${OPENCODE_ARGS[@]}"
