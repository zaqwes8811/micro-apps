#!/usr/bin/env bash
# Helper only — does not start work by itself unless you execute it.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

MAX_ITERATIONS="${MAX_ITERATIONS:-10}"
AGENT="${AGENT:-opencode}"

if ! command -v ralph >/dev/null 2>&1; then
  echo "ralph not found. Install once:"
  echo "  npm install -g @th0rgal/ralph-wiggum"
  exit 1
fi

if ! command -v opencode >/dev/null 2>&1; then
  echo "opencode not found in PATH (needed for --agent opencode)"
  exit 1
fi

echo "cwd: $ROOT"
echo "agent: $AGENT"
echo "max-iterations: $MAX_ITERATIONS"
echo "prompt: $ROOT/PROMPT.md"
echo
echo "Prerequisites (you): turbo3 on :8080, OpenCode lite synced."
echo

exec ralph \
  --prompt-file "$ROOT/PROMPT.md" \
  --agent "$AGENT" \
  --max-iterations "$MAX_ITERATIONS" \
  --completion-promise COMPLETE \
  "$@"
