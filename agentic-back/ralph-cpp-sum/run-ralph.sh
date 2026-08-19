#!/usr/bin/env bash
# Helper only — does not start work by itself unless you execute it.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

# ralph 1.2.x requires Bun; nvm npm global bin may not include it in non-login shells
export PATH="${BUN_INSTALL:-$HOME/.bun}/bin:${PATH}"

MAX_ITERATIONS="${MAX_ITERATIONS:-25}"
AGENT="${AGENT:-opencode}"
OPENCODE_AGENT="${OPENCODE_AGENT:-ralph}"
LAST_ACTIVITY_TIMEOUT="${LAST_ACTIVITY_TIMEOUT:-5m}"

if ! command -v ralph >/dev/null 2>&1; then
  echo "ralph not found. Install once (nvm):"
  echo "  npm uninstall -g @th0rgal/ralph-wiggum  # if ENOTEMPTY on upgrade"
  echo "  rm -rf \"\$(npm root -g)/@th0rgal/ralph-wiggum\""
  echo "  npm install -g @th0rgal/ralph-wiggum@latest"
  exit 1
fi

if ! command -v bun >/dev/null 2>&1; then
  echo "bun not found (required by ralph). Install: https://bun.sh"
  exit 1
fi

if ! command -v opencode >/dev/null 2>&1; then
  echo "opencode not found in PATH (needed for --agent opencode)"
  exit 1
fi

RALPH_HELP="$(ralph --help 2>&1 || true)"
TIMEOUT_ARGS=()
if [[ "$RALPH_HELP" == *"--last-activity-timeout"* ]]; then
  TIMEOUT_ARGS=(--last-activity-timeout "$LAST_ACTIVITY_TIMEOUT")
fi

# Pass OpenCode sub-agent after -- unless caller already did.
if [[ $# -ge 1 && "$1" == "--" ]]; then
  OPENCODE_ARGS=("$@")
else
  OPENCODE_ARGS=(-- --agent "$OPENCODE_AGENT" "$@")
fi

echo "cwd: $ROOT"
echo "ralph agent: $AGENT"
echo "opencode agent: $OPENCODE_AGENT (override: OPENCODE_AGENT=lite or ./run-ralph.sh -- --agent lite)"
echo "max-iterations: $MAX_ITERATIONS"
if ((${#TIMEOUT_ARGS[@]})); then
  echo "last-activity-timeout: $LAST_ACTIVITY_TIMEOUT"
else
  echo "last-activity-timeout: (not in npm @latest 1.2.2 — only in GitHub master; see README)"
fi
echo "prompt: $ROOT/PROMPT.md"
echo
echo "Prerequisites (you): turbo3 on :8080, OpenCode lite synced, timeout 600s in opencode.json."
echo "If llama tab closes or exit 137: restart llamacpp-serve-qwen35-4b-turbo3.sh, then re-run."
echo

exec ralph \
  --prompt-file "$ROOT/PROMPT.md" \
  --agent "$AGENT" \
  --max-iterations "$MAX_ITERATIONS" \
  --completion-promise COMPLETE \
  --no-commit \
  "${TIMEOUT_ARGS[@]}" \
  "${OPENCODE_ARGS[@]}"
