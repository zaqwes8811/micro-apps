# PRD: CsvReader + gtest

## Goal

Add a C++ **`CsvReader`** class that reads comma-separated values files, with gtest coverage and ctest registration. All existing sum/sub work must keep passing.

## Scope

- **In:** `CsvReader` API, fixture, `test_csv_reader.cpp`, CMake/ctest wiring for `csv_reader_test`
- **Out:** third-party CSV libraries, quoted fields, streaming large files, changes to sum/sub logic
- **Baseline (done — do not regress):** `ralph_sum.hpp`, `test_ralph_sum.cpp`, compile-time bool rejection in `CMakeLists.txt`, `ralph_sum_test` in ctest

## Requirements

### 1. CsvReader class (`csv_reader.hpp` + `csv_reader.cpp`)

```cpp
class CsvReader {
public:
  explicit CsvReader(const std::string& path, bool has_header);

  bool is_open() const;                          // true if file opened successfully
  bool read_row(std::vector<std::string>& cols); // next row → cols; false at EOF
  void reset();                                  // rewind to start (respects has_header)
};
```

**Behaviour:**

- Constructor takes a **file path** and **`has_header`** — whether the first non-empty line of the file is a column header row.
- **`has_header == true`:** after open or **`reset`**, the header line is **skipped**; the first **`read_row`** returns the first **data** row. Subsequent **`read_row`** calls return following data rows until EOF.
- **`has_header == false`:** no line is skipped; **`read_row`** returns every row including the first line.
- **`read_row`** splits one line on **commas** into column strings (trim trailing `\r` if present).
- Empty lines are **skipped** (never counted as header or data).
- On missing/unreadable file: **`is_open()`** returns `false`; **`read_row`** returns `false`.
- Use only **standard library** (`<fstream>`, `<string>`, `<vector>`).

### 2. Fixture

Create **`test_data/sample.csv`**:

```csv
name,age,city
Alice,30,NYC
Bob,25,LA
```

### 3. gtest — `test_csv_reader.cpp` → target `csv_reader_test`

Register with ctest as **`csv_reader_test`**.

All tests on `test_data/sample.csv` use **`has_header = true`** (second constructor argument).

| Test | Assert |
|------|--------|
| Opens valid fixture | `CsvReader("test_data/sample.csv", true)` → `is_open()` true |
| Reads rows | first `read_row` → `Alice,30,NYC`; second → `Bob,25,LA`; third → `false` (header not returned) |
| EOF | after 2 data rows, next `read_row` → `false` |
| Missing file | `CsvReader("nonexistent.csv", true)` → `is_open()` false; `read_row` → false |
| Reset | read all data rows, `reset()`, first `read_row` → `Alice,...` again (header skipped again) |

**Paths:** use `"test_data/sample.csv"` — never `"../test_data/..."`.

### 4. CMake / ctest

- `add_executable(csv_reader_test …)` links gtest.
- Register with `add_test(NAME csv_reader_test … WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})`.
- Do **not** break existing `ralph_sum_test` or bool-rejection checks.

**Working directory:** runtime tests always run with **cwd = project root**, not `build/`.

## Constraints

- **C++20** preferred (`CMAKE_CXX_STANDARD 20`).
- CSV fixture paths in test source: relative to project root only.
- Invoke **ctest from project root** — do not verify by running `./build/csv_reader_test` directly.

## Acceptance criteria

All **five** must be true simultaneously (verified by `./verify.sh` exit 0):

1. `CsvReader` implements the API and behaviour above.
2. `test_data/sample.csv` exists with header + 2 data rows.
3. `csv_reader_test` covers all cases in the table above.
4. **Compile-time:** configure output includes `Bool rejection test PASSED` (sum/sub with bool rejected).
5. **Runtime:** ctest output includes `100% tests passed, 0 tests failed out of 2` — both `ralph_sum_test` and `csv_reader_test` **Passed**.

Timeout (exit 124), verify > 15s, `***Failed`, or `Subprocess killed` = **not done** — infinite loop or bug; fix code.

## Completion promise

<promise>COMPLETE</promise>

Emit **only** when criteria 1–5 pass **and** you ran `./verify.sh` in the **same iteration** with exit 0. See `PROMPT.md` → **STOP — COMPLETE gate**. Never promise from memory or without fresh verify output.
