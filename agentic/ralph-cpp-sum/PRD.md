# PRD: template sum + gtest

| ID | Item | Done when |
|----|------|-----------|
| 1 | Template sum | C++20 `requires` preferred; else C++17 SFINAE; **`bool` rejected** at compile time |
| 2 | gtest | `test_ralph_sum.cpp` → `ralph_sum_test`; int, float, double, etc.; **no bool** |
| 3 | Compile-fail | `test_bool_rejection.cpp`; CMake `CheckCXXSourceCompiles` — **`BOOL_SUM_COMPILES` must be false** |
| 4 | Build | `cmake -S . -B build && cmake --build build && ctest` succeeds (C++20 or C++17) |
| 5 | Green | Runtime tests pass; bool-rejection check passes |

Completion promise: `COMPLETE` (see `PROMPT.md`).
