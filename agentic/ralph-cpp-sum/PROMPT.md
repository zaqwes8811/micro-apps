# Task: C++ template sum + gtest

Work only in this directory. Do not touch sibling projects under `agentic/`.

## Goal

1. Implement a C++ **template** function that returns the sum of two numbers.
2. Write **two test artifacts** (see below).
3. Provide a CMake build that finds/links gtest and runs both checks.
4. Build and run everything. All checks must pass.

## Two tests (both required)

### 1. Runtime gtest — `test_ralph_sum.cpp` → target `ralph_sum_test`

- gtest cases for numeric types: int, float, double, and at least one more (e.g. `long long`, `char`).
- **No** calls to `sum` with `bool` in this file.

### 2. Compile-time bool rejection — `test_bool_rejection.cpp`

- Separate source file with code that **must fail to compile**, e.g. `sum(true, false)`.
- **Do not** add it as a normal `add_executable` target (that would break the build).
- Wire the compile-fail check in **CMake** using `CheckCXXSourceCompiles` (preferred):

```cmake
include(CheckCXXSourceCompiles)

# Passes when sum(bool, bool) does NOT compile
check_cxx_source_compiles("
#include \"ralph_sum.hpp\"
int main() { (void)sum(true, false); return 0; }
" BOOL_SUM_COMPILES
  CXX_STANDARD 20
)

if(BOOL_SUM_COMPILES)
  message(FATAL_ERROR \"bool sum must not compile\")
endif()
```

- Invert the logic: **`check_cxx_source_compiles` succeeds → test fails**; **`BOOL_SUM_COMPILES` is false → test passes**.
- You may embed the snippet above directly, or read equivalent code from `test_bool_rejection.cpp` into the string.
- Register with `enable_testing()` + `add_test` / `ctest` if needed so `cmake --build build && ctest` runs both runtime and compile-fail checks.

- A passing project means: `ralph_sum_test` runs green **and** the bool-rejection check confirms `sum(bool, …)` does not compile.

## Constraints

- **C++ standard** — prefer **C++20** (`CMAKE_CXX_STANDARD 20`, `requires` to constrain the template). If the toolchain does not support C++20, fall back to **C++17** (`CMAKE_CXX_STANDARD 17`) and use SFINAE / `std::enable_if` / `static_assert` for the same constraints.
- **No `bool`** — `sum` must accept numeric types (integers, floating-point, etc.) but **must reject `bool`** at compile time. Examples:
  - C++20: `requires std::is_arithmetic_v<T> && !std::same_as<T, bool>`
  - C++17: `std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value>` (or equivalent SFINAE)
- Do **not** call `sum` with `bool` in `test_ralph_sum.cpp` — bool rejection belongs only in `test_bool_rejection.cpp` (compile-fail).
- Header + tests are enough; no need for a separate main app.
- Prefer system `libgtest-dev` / CMake `find_package(GTest)` or FetchContent if needed.
- Expected layout: `CMakeLists.txt`, `ralph_sum.hpp`, `test_ralph_sum.cpp`, `test_bool_rejection.cpp`.
- After each meaningful change: build, run tests, fix failures.
- Do not run git commands (disabled in OpenCode); the human commits from the parent repo.

## Checklist (update `progress.md` each iteration)

- [ ] Template sum with type constraint (C++20 `requires`, or C++17 SFINAE); numeric types, **not bool**
- [ ] gtest target `ralph_sum_test` (int, float, double, etc. — no bool)
- [ ] Compile-fail test `test_bool_rejection.cpp` wired in CMake (bool call must not compile)
- [ ] CMake builds and **ctest** / test run passes (runtime + compile-fail check)
- [ ] All tests pass

## Done criteria

All checklist items are done **and**:

- `./build/ralph_sum_test` (or `ctest`) exits 0
- bool-rejection compile-fail test passes (i.e. `sum(bool, …)` correctly rejected)

Only then output exactly:

<promise>COMPLETE</promise>

Do **not** emit the promise if any test fails or the project does not build.
