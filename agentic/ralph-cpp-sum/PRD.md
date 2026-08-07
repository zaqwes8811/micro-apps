# PRD: template sum + gtest

| ID | Item | Done when |
|----|------|-----------|
| 1 | Template sum | `template <typename T> T sum(T a, T b)` (or equivalent) in a header |
| 2 | gtest | Tests cover int, floating, and one more type; edge cases optional |
| 3 | Build | `cmake -S . -B build && cmake --build build` succeeds |
| 4 | Green | Test binary runs with exit code 0 |

Completion promise: `COMPLETE` (see `PROMPT.md`).
