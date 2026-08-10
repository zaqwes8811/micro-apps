# Progress

## Status

- [ ] **`CsvReader`** — constructor `(path, has_header)`; skip header when `has_header == true`; `read_row`, `reset`, EOF, missing file
- [ ] **`test_data/sample.csv`** fixture (header + 2 data rows)
- [ ] gtest **`csv_reader_test`** — `has_header=true` on fixture; opens, data rows, EOF, missing file, reset
- [ ] CMake: `csv_reader_test` registered with `WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}`
- [ ] **Runtime:** `ctest --test-dir build` — **2 tests** pass (`ralph_sum_test` + `csv_reader_test`)
- [ ] **Full verify:** configure + build + `ctest --test-dir build` exits 0 (no sum/sub regression)

## Log

<!-- Ralph: append iteration notes below -->

Scope: **CsvReader only**. Sum/sub + bool-rejection done — do not regress.

Harness split 2026-08-10: task spec → `PRD.md`, stable rules → `PROMPT.md`, checklist here.
