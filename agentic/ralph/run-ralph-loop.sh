#!/usr/bin/env bash
# Direct Ralph-style loop: re-read PROMPT.md every iteration (no @th0rgal/ralph-wiggum).
# cwd = agentic/; harness = agentic/ralph/; OpenCode via ralph/bin/opencode wrapper.
set -euo pipefail

HARNESS="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKDIR="$(cd "$HARNESS/.." && pwd)"
cd "$WORKDIR"

export PATH="${BUN_INSTALL:-$HOME/.bun}/bin:${PATH}"

MAX_ITERATIONS="${MAX_ITERATIONS:-25}"
OPENCODE_AGENT="${OPENCODE_AGENT:-ralph-4b}"
COMPLETION_PROMISE="${COMPLETION_PROMISE:-IDLE}"
PROMPT_FILE="${PROMPT_FILE:-$HARNESS/PROMPT.md}"
OPENCODE_BIN="$HARNESS/bin/opencode"

if ! command -v bun >/dev/null 2>&1; then
	echo "bun not found. Install: https://bun.sh"
	exit 1
fi
if ! command -v opencode >/dev/null 2>&1; then
	echo "opencode not found in PATH"
	exit 1
fi
if [[ ! -f "$PROMPT_FILE" ]]; then
	echo "prompt file not found: $PROMPT_FILE" >&2
	exit 1
fi

export REAL_OPENCODE="$(command -v opencode)"
export RALPH_OPENCODE_USER_CONFIG="${HOME}/.config/opencode/opencode.json"
export RALPH_RUNTIME_CONFIG="${RALPH_RUNTIME_CONFIG:-$(dirname "$RALPH_OPENCODE_USER_CONFIG")/ralph-loop.runtime.json}"
chmod +x "$OPENCODE_BIN" "$HARNESS/bin/opencode-runtime-config.py"

EXTRA_OPENCODE_ARGS=()
if [[ $# -ge 1 && "$1" == "--" ]]; then
	shift
	EXTRA_OPENCODE_ARGS=("$@")
fi

echo "cwd: $WORKDIR"
echo "harness: $HARNESS"
echo "mode: direct loop (re-read prompt each iteration)"
echo "opencode agent: $OPENCODE_AGENT"
echo "OPENCODE_CONFIG: $RALPH_RUNTIME_CONFIG (from $RALPH_OPENCODE_USER_CONFIG)"
python3 "$HARNESS/bin/opencode-runtime-config.py" diff "$RALPH_OPENCODE_USER_CONFIG"
echo "opencode via: $OPENCODE_BIN -> $REAL_OPENCODE"
echo "max-iterations: $MAX_ITERATIONS"
echo "completion-promise: $COMPLETION_PROMISE"
echo "prompt: $PROMPT_FILE"
echo

if [[ "$(./ralph/bin/list-open.sh 2>/dev/null || echo STOP)" == "IDLE" ]]; then
	echo "Completion: $COMPLETION_PROMISE — no open tasks."
	exit 0
fi

for ((i = 1; i <= MAX_ITERATIONS; i++)); do
	echo "=== iteration $i / $MAX_ITERATIONS ==="

	PROMPT="$(cat "$PROMPT_FILE")"
	if [[ -z "${PROMPT//[[:space:]]/}" ]]; then
		echo "prompt file empty: $PROMPT_FILE" >&2
		exit 1
	fi

	if [[ "$(./ralph/bin/list-open.sh 2>/dev/null || echo STOP)" == "IDLE" ]]; then
		echo "Completion: $COMPLETION_PROMISE — no open tasks."
		exit 0
	fi

	out="$("$OPENCODE_BIN" run --agent "$OPENCODE_AGENT" "${EXTRA_OPENCODE_ARGS[@]}" "$PROMPT" 2>&1)" || true
	printf '%s\n' "$out"

	if [[ "$(./ralph/bin/list-open.sh 2>/dev/null || echo STOP)" == "IDLE" ]]; then
		echo "Completion: $COMPLETION_PROMISE — no open tasks."
		exit 0
	fi
done

echo "Max iterations reached ($MAX_ITERATIONS)" >&2
exit 1
