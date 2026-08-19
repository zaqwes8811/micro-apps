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
PROMPT_TEMPLATE="${PROMPT_TEMPLATE:-$HARNESS/prompt.template.md}"

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
export RALPH_OPENCODE_USER_CONFIG="${HOME}/.config/opencode/opencode.json"
export RALPH_RUNTIME_CONFIG="${RALPH_RUNTIME_CONFIG:-$(dirname "$RALPH_OPENCODE_USER_CONFIG")/ralph-loop.runtime.json}"
chmod +x "$RALPH_OPENCODE_BINARY" "$HARNESS/bin/opencode-runtime-config.py"

# Ralph 1.2.2 hashes every `git ls-files` path in cwd before spawning OpenCode.
# Point Ralph git at the tiny harness repo so it does not walk the workdir tree.
if [[ ! -d "$HARNESS/.git" ]]; then
  git -C "$HARNESS" init -q
fi
export GIT_DIR="$HARNESS/.git"
export GIT_WORK_TREE="$HARNESS"

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
echo "OPENCODE_CONFIG: $RALPH_RUNTIME_CONFIG (from $RALPH_OPENCODE_USER_CONFIG)"
python3 "$HARNESS/bin/opencode-runtime-config.py" diff "$RALPH_OPENCODE_USER_CONFIG"
echo "opencode via: $RALPH_OPENCODE_BINARY -> $REAL_OPENCODE"
echo "max-iterations: $MAX_ITERATIONS"
echo "Ralph git: $GIT_WORK_TREE (not workdir ls-files)"
echo "prompt: $PROMPT_FILE"
echo "prompt-template: $PROMPT_TEMPLATE"
echo

# if [[ "$(./ralph/bin/list-open.sh 2>/dev/null || echo STOP)" == "IDLE" ]]; then
#   echo "Completion: IDLE — no open tasks."
#   exit 0
# fi

exec ralph \
  --prompt-file "$PROMPT_FILE" \
  --prompt-template "$PROMPT_TEMPLATE" \
  --agent "$AGENT" \
  --max-iterations "$MAX_ITERATIONS" \
  --completion-promise "IDLE" \
  --no-commit \
  "${TIMEOUT_ARGS[@]}" \
  "${OPENCODE_ARGS[@]}"
