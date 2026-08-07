# Task: C++ template sum + sub + gtest

**Workdir (sandbox):** `/home/zaqwes/My/micro-apps/agentic/ralph-cpp-sum`

Stay **inside this directory only**. Do not leave it for any reason.

## Constraints

- **Sandbox** — all file reads/writes and shell commands must stay under the workdir above.
  - Do **not** `cd ..`, `cd` to `agentic/`, home, `/tmp`, or any path outside the workdir.
  - Do **not** read, edit, or create files in sibling folders (`../small`, `../to-micro-apps`, etc.).
  - Use **relative paths** from the workdir (`./ralph_sum.hpp`, `./build/`, not `~/My/micro-apps/...` elsewhere).
  - Bash: run builds/tests as `cmake -S . -B build` from `.` — never from parent repos.
  - If a tool requires an absolute path, it must still point **only** inside `ralph-cpp-sum/`.

## Goal

1. Implement C++ **template** functions **`sum`** and **`sub`** (addition and subtraction).
2. Write **two test artifacts** (see below).
3. Provide a CMake build that finds/links gtest and runs both checks.
4. Build and run everything. All checks must pass.

## Functions (`ralph_sum.hpp`)

Both functions share the same type constraint:

```cpp
template <typename T>
requires (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
constexpr T sum(T a, T b) { return a + b; }

template <typename T>
requires (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
constexpr T sub(T a, T b) { return a - b; }
```

## Two tests (both required)

### 1. Runtime gtest — `test_ralph_sum.cpp` → target `ralph_sum_test`

- gtest for **`sum`** and **`sub`**: int, float, double, and at least one more type (e.g. `long long`, `char`).
- Include negative cases where useful (e.g. `sub(3, 5) == -2`).
- **No** calls to `sum` or `sub` with `bool` in this file.

### 2. Compile-time bool rejection — `test_bool_rejection.cpp`

- Reference source with code that **must fail to compile**, e.g. `sum(true, false)` and `sub(true, false)`.
- **Do not** add it as a normal `add_executable` target (that would break the build).
- Wire the compile-fail check in **CMake** using `CheckCXXSourceCompiles` (preferred):

```cmake
include(CheckCXXSourceCompiles)

# Passes when sum/sub(bool, bool) do NOT compile
check_cxx_source_compiles("
#include \"ralph_sum.hpp\"
int main() {
  (void)sum(true, false);
  (void)sub(true, false);
  return 0;
}
" BOOL_MATH_COMPILES
  CXX_STANDARD 20
)

if(BOOL_MATH_COMPILES)
  message(FATAL_ERROR \"sum/sub with bool must not compile\")
endif()
```

- Invert the logic: **`check_cxx_source_compiles` succeeds → test fails**; **`BOOL_MATH_COMPILES` is false → test passes**.
- Register with `enable_testing()` + `add_test(NAME ralph_sum_test …)` so **ctest** runs the runtime suite (do not verify by calling `./build/ralph_sum_test` directly).

- A passing project means: **ctest** runtime green **and** bool-rejection confirms `sum`/`sub` with `bool` do not compile.

## Build & tool constraints

- **C++ standard** — prefer **C++20** (`CMAKE_CXX_STANDARD 20`, `requires`). If unsupported, fall back to **C++17** + SFINAE / `enable_if`.
- **No `bool`** — `sum` and `sub` accept numeric types but **reject `bool`** at compile time.
- Do **not** call `sum`/`sub` with `bool` in `test_ralph_sum.cpp` — bool rejection is compile-time only.
- Expected layout: `CMakeLists.txt`, `ralph_sum.hpp`, `test_ralph_sum.cpp`, `test_bool_rejection.cpp`.
- After each meaningful change: build, run tests, fix failures.
- Do not run git commands (disabled in OpenCode); the human commits from the parent repo.

## Checklist (update `progress.md` each iteration)

- [ ] Template **`sum`** and **`sub`** with type constraint (C++20 `requires` or C++17 SFINAE); **not bool**
- [ ] gtest covers **both** `sum` and `sub` (int, float, double, etc.)
- [ ] **Runtime:** `ctest --test-dir build` passes
- [ ] **Compile-time:** `BOOL_MATH_COMPILES` is false (sum **and** sub reject bool)
- [ ] Full verify: configure + build + `ctest --test-dir build` exits 0

## Done criteria

Task is complete **only when both** test layers pass:

| Layer | Verify | Must |
|-------|--------|------|
| **Runtime** | `ctest --test-dir build --output-on-failure` | exit 0; tests listed and passed |
| **Compile-time** | `cmake -S . -B build` | `BOOL_MATH_COMPILES` failed + *Bool rejection test PASSED* |

### How to run tests (correct)

**Always use ctest** — do **not** verify runtime by running `./build/ralph_sum_test` directly.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

**Wrong:**

```bash
./build/ralph_sum_test          # ❌ bypasses ctest
ctest --output-on-failure       # ❌ from root without --test-dir build
```

### STOP — output promise immediately

When **both** are true, your **next reply** must be only verification summary + `<promise>COMPLETE</promise>`:

1. `cmake -S . -B build` → *Bool rejection test PASSED*
2. `ctest --test-dir build --output-on-failure` → exit 0, tests passed

**Do not** keep working after that.

Verify before promising:

```bash
cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure
```

Only when **runtime AND compile-time tests pass** output exactly:

<promise>COMPLETE</promise>

Do **not** emit the promise if:
- any gtest fails,
- `sum` or `sub` with `bool` compiles when it should not,
- build/configure fails,
- only one of the two test layers was verified.
