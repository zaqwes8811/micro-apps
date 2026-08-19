/**
 * @file test_csv_reader.cpp
 * @brief Gtest tests for CsvReader class
 */

#include "csv_reader.hpp"
#include <gtest/gtest.h>

// Tests for opening valid fixture
TEST(CsvReaderTest, OpensValidFixture) {
  CsvReader reader("test_data/sample.csv", true);
  EXPECT_TRUE(reader.is_open());
}

// Tests for reading rows (has_header=true, header skipped)
TEST(CsvReaderTest, ReadsRows) {
  CsvReader reader("test_data/sample.csv", true);
  EXPECT_TRUE(reader.is_open());

  std::vector<std::string> cols;

  // First row should be first data row (header is skipped)
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols.size(), 3u);
  EXPECT_EQ(cols[0], "Alice");
  EXPECT_EQ(cols[1], "30");
  EXPECT_EQ(cols[2], "NYC");

  // Second row - data
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols.size(), 3u);
  EXPECT_EQ(cols[0], "Bob");
  EXPECT_EQ(cols[1], "25");
  EXPECT_EQ(cols[2], "LA");

  // Third read should be EOF
  EXPECT_FALSE(reader.read_row(cols));
}

// Tests for EOF (has_header=true)
TEST(CsvReaderTest, NoMoreRows) {
  CsvReader reader("test_data/sample.csv", true);
  EXPECT_TRUE(reader.is_open());

  std::vector<std::string> cols;

  // Read all data rows (header is skipped internally)
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "Alice");
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "Bob");
  EXPECT_FALSE(reader.read_row(cols)); // EOF
}

// Tests for missing file
TEST(CsvReaderTest, MissingFile) {
  CsvReader reader("nonexistent.csv", true);
  EXPECT_FALSE(reader.is_open());
  std::vector<std::string> cols;
  EXPECT_FALSE(reader.read_row(cols));
}

// Tests for reset functionality (has_header=true, reset resets to start, header skipped again)
TEST(CsvReaderTest, Reset) {
  CsvReader reader("test_data/sample.csv", true);
  EXPECT_TRUE(reader.is_open());

  std::vector<std::string> cols;

  // Header is skipped in constructor, first read_row returns first data row
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "Alice");

  // Read second data row
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "Bob");

  // After reading all rows, reset should give us header again
  reader.reset();
  EXPECT_FALSE(reader.read_row(cols)); // header skipped again
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "Alice");
}
