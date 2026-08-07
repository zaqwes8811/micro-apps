# PRD: template sum + sub + gtest

| ID | Item | Done when |
|----|------|-----------|
| 1 | Template sum + sub | C++20 `requires` (or C++17 SFINAE); **`bool` rejected** for both |
| 2 | gtest | `test_ralph_sum.cpp` → tests for **sum and sub**; int, float, double, etc.; **no bool** |
| 3 | Compile-fail | CMake `CheckCXXSourceCompiles` — **`BOOL_MATH_COMPILES` must be false** (sum + sub with bool) |
| 4 | Build | `cmake -S . -B build && cmake --build build` succeeds |
| 5 | Green | `ctest --test-dir build` pass **and** compile-time bool-rejection pass |

Completion promise: `COMPLETE` only when **ctest runtime AND compile-time** pass (see `PROMPT.md`).
