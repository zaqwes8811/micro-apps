#!/usr/bin/env bash
# Run OpenCode from the harness dir (tiny git), not from agentic workdir.
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
# Installed as ralph/bin/opencode → harness is parent of bin/
if [[ "$(basename "$HERE")" == "bin" ]]; then
  HARNESS="$(cd "$HERE/.." && pwd)"
else
  HARNESS="$HERE"
fi

echo "$(date -Is) wrap cwd=$HARNESS argv=$*" >> "$HARNESS/wrap.log"

USER_CFG="${HOME}/.config/opencode/opencode.json"
OUT="${RALPH_RUNTIME_CONFIG:-$HARNESS/.opencode.runtime.json}"
mkdir -p "$(dirname "$OUT")"

python3 - "$USER_CFG" "${OPENCODE_CONFIG:-}" "$OUT" <<'PY'
import json, sys
from pathlib import Path

def load(p):
    path = Path(p) if p else None
    if not path or not path.is_file():
        return {}
    return json.loads(path.read_text(encoding="utf-8"))

user, stub, out = sys.argv[1], sys.argv[2], sys.argv[3]
cfg = load(user)
cfg.update(load(stub))
cfg["snapshot"] = False
cfg["lsp"] = False
cfg["watcher"] = {"ignore": ["**/*"]}
Path(out).write_text(json.dumps(cfg, indent=2) + "\n", encoding="utf-8")
PY

export OPENCODE_CONFIG="$OUT"
cd "$HARNESS"
exec "${REAL_OPENCODE:?REAL_OPENCODE not set}" "$@"
