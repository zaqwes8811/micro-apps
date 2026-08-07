# Task: C++ template sum + gtest

Work only in this directory. Do not touch sibling projects under `agentic/`.

## Goal

1. Implement a C++ **template** function that returns the sum of two numbers.
2. Write **gtest** tests for it (ints, floats, and at least one other numeric type).
3. Provide a CMake build that finds/links gtest and builds a test binary.
4. Build and run the tests. They must pass.

## Constraints

- Header + tests are enough; no need for a separate main app.
- Prefer system `libgtest-dev` / CMake `find_package(GTest)` or FetchContent if needed.
- Keep the project minimal: `CMakeLists.txt`, one header, one test source.
- After each meaningful change: build, run tests, fix failures.
- Commit progress in small steps (ralph may also auto-commit).

## Checklist (update `progress.md` each iteration)

- [ ] Template sum function exists
- [ ] gtest tests written
- [ ] CMake builds the test target
- [ ] All tests pass

## Done criteria

All checklist items are done **and** running the test binary exits 0.

Only then output exactly:

<promise>COMPLETE</promise>

Do **not** emit the promise if any test fails or the project does not build.
