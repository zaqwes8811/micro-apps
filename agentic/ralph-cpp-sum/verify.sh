#!/usr/bin/env bash
# Configure, build, test — each runtime test capped at 5s (see CMakeLists.txt).
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

# Stale paths from before binaries moved to build/.internal/
rm -f build/csv_reader_test build/ralph_sum_test

cmake -S . -B build
cmake --build build
# Hard cap for whole run: 2 × 5s tests + overhead. Exit 124 = hung test / infinite loop.
timeout 15 ctest --test-dir build --output-on-failure
