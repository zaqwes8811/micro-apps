# PRD: CsvReader + gtest

## Goal

C++ **`CsvReader`** reads comma-separated values files, with gtest coverage and ctest registration. All existing sum/sub work must keep passing.

## Scope

- **In:** `CsvReader` API, fixture, `test_csv_reader.cpp`, CMake/ctest wiring for `csv_reader_test`
- **Out:** third-party CSV libraries, quoted fields, streaming large files, changes to sum/sub logic
- **Baseline (done — do not regress):** `ralph_sum.hpp`, `test_ralph_sum.cpp`, compile-time bool rejection in `CMakeLists.txt`, `ralph_sum_test` in ctest

## Requirements

### 1. CsvReader class (`csv_reader.hpp` + `csv_reader.cpp`)

```cpp
class CsvReader {
public:
  explicit CsvReader(const std::string& path, bool has_header = false);

  bool is_open() const;
  bool read_row(std::vector<std::string>& cols);
  void reset();
};
```

**Behaviour:**

- Constructor takes a **file path** and optional **`has_header`**. When `has_header == true`, the header line is skipped before the first data row.
- **`read_row`** splits one line on **commas** into column strings (trim trailing `\r` if present).
- Empty lines are **skipped** (return `false`, read next line on subsequent call).
- On missing/unreadable file: **`is_open()`** returns `false`; **`read_row`** returns `false`.
- **`reset()`** rewinds to the start of the file; header skip applies again when `has_header == true`.
- Use only **standard library** (`<fstream>`, `<string>`, `<vector>`).

### 2. Fixture

**`test_data/sample.csv`:**

```csv
name,age,city
Alice,30,NYC
Bob,25,LA
```

### 3. gtest — `test_csv_reader.cpp` → target `csv_reader_test`

| Test | Assert |
|------|--------|
| Opens valid fixture | `is_open()` true for `test_data/sample.csv` with `has_header=true` |
| Reads rows | With `has_header=true`: 2 data rows (Alice, Bob); columns match fixture |
| EOF | `read_row` returns `false` after last data row |
| Missing file | `is_open()` false; `read_row` returns false |
| Reset | After reading all rows, `reset()` then reads from start (header skipped again → first data row) |

**Paths:** `"test_data/sample.csv"` — never `"../test_data/..."`.

### 4. CMake / ctest

- `add_executable(csv_reader_test …)` links gtest.
- Register with `add_test(NAME csv_reader_test … WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})`.
- Do **not** break existing `ralph_sum_test` or bool-rejection checks.

## Constraints

- **C++20** (`CMAKE_CXX_STANDARD 20`).
- Verify only via **`./verify.sh`** from project root.

## Acceptance criteria

1. `CsvReader` implements the API and behaviour above.
2. `test_data/sample.csv` exists with header + 2 data rows.
3. `csv_reader_test` covers all cases in the table above.
4. `./verify.sh` → exit 0, **2 tests** passed, `Bool rejection test PASSED`.
5. Sum/sub tests have not regressed.

## Completion promise

<promise>COMPLETE</promise>

Emit only when **all five** acceptance criteria pass.
