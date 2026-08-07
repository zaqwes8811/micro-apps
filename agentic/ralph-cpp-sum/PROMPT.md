# Task: C++ template sum + gtest

**Workdir (sandbox):** `/home/zaqwes/My/micro-apps/agentic/ralph-cpp-sum`

Stay **inside this directory only**. Do not leave it for any reason.

## Constraints

- **Sandbox** — all file reads/writes and shell commands must stay under the workdir above.
  - Do **not** `cd ..`, `cd` to `agentic/`, home, `/tmp`, or any path outside the workdir.
  - Do **not** read, edit, or create files in sibling folders (`../small`, `../to-micro-apps`, etc.).
  - Use **relative paths** from the workdir (`./ralph_sum.hpp`, `./build/`, not `~/My/micro-apps/...` elsewhere).
  - Bash: run builds/tests as `./build/...` or `cmake -S . -B build` from `.` — never from parent repos.
  - If a tool requires an absolute path, it must still point **only** inside `ralph-cpp-sum/`.

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
- Register with `enable_testing()` + `add_test(NAME ralph_sum_test …)` so **ctest** runs the runtime suite (do not verify by calling `./build/ralph_sum_test` directly).

- A passing project means: **ctest** runtime green **and** the bool-rejection check confirms `sum(bool, …)` does not compile.

## Build & tool constraints

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
- [ ] **Runtime:** `ctest` passes (gtest via `add_test`, not manual `./build/ralph_sum_test`)
- [ ] **Compile-time:** bool-rejection check in CMake passes (`BOOL_SUM_COMPILES` is false)
- [ ] Full verify: configure + build + `ctest --test-dir build` exits 0

## Done criteria

Task is complete **only when both** test layers pass:

| Layer | Verify | Must |
|-------|--------|------|
| **Runtime** | `ctest --test-dir build --output-on-failure` | exit 0; tests listed and passed |
| **Compile-time** | `cmake -S . -B build` (configure step) | `BOOL_SUM_COMPILES` failed + status *Bool rejection test PASSED* |

### How to run tests (correct)

**Always use ctest** — do **not** verify runtime by running `./build/ralph_sum_test` directly.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Alternative (same thing):

```bash
cd build && ctest --output-on-failure
```

**Wrong** (do not use for done verification):

```bash
./build/ralph_sum_test          # ❌ bypasses ctest registration
ctest --output-on-failure       # ❌ from project root — finds no tests
```

Plain `ctest` from `.` without `--test-dir build` prints *No tests were found!!!* — use `--test-dir build`, not bare `ctest`.

### STOP — output promise immediately

When **both** are true, your **next reply** must be only verification summary + `<promise>COMPLETE</promise>`:

1. `cmake -S . -B build` → *Bool rejection test PASSED*
2. `ctest --test-dir build --output-on-failure` → exit 0, tests passed

**Do not** keep working after that. **Do not** edit CMakeLists further.

Verify before promising:

```bash
cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure
```

Compile-time bool check runs at **configure** (`cmake -S . -B build`).

Only when **runtime AND compile-time tests pass** output exactly:

<promise>COMPLETE</promise>

Do **not** emit the promise if:
- any gtest fails,
- bool sum compiles when it should not,
- build/configure fails,
- only one of the two test layers was verified.
