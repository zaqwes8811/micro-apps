# PRD: template sum + gtest

| ID | Item | Done when |
|----|------|-----------|
| 1 | Template sum | C++20 `requires` preferred; else C++17 SFINAE; **`bool` rejected** at compile time |
| 2 | gtest | `test_ralph_sum.cpp` → `ralph_sum_test`; int, float, double, etc.; **no bool** |
| 3 | Compile-fail | `test_bool_rejection.cpp`; CMake `CheckCXXSourceCompiles` — **`BOOL_SUM_COMPILES` must be false** |
| 4 | Build | `cmake -S . -B build && cmake --build build && ctest` succeeds (C++20 or C++17) |
| 5 | Green | **Both:** `ctest --test-dir build` pass **and** compile-time bool-rejection pass |

Completion promise: `COMPLETE` only when **ctest runtime AND compile-time** pass (see `PROMPT.md`). Do not verify via `./build/ralph_sum_test`.
