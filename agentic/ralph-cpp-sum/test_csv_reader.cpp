/**
 * @file test_csv_reader.cpp
 * @brief Gtest tests for CsvReader class
 */

#include "csv_reader.hpp"
#include <gtest/gtest.h>

// Tests for opening valid fixture
TEST(CsvReaderTest, OpensValidFixture) {
  CsvReader reader("test_data/sample.csv");
  EXPECT_TRUE(reader.is_open());
}

// Tests for reading rows
TEST(CsvReaderTest, ReadsRows) {
  CsvReader reader("test_data/sample.csv");
  EXPECT_TRUE(reader.is_open());

  std::vector<std::string> cols;

  // First row should be header
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols.size(), 3u);
  EXPECT_EQ(cols[0], "name");
  EXPECT_EQ(cols[1], "age");
  EXPECT_EQ(cols[2], "city");

  // Second row - data
  reader.reset();
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols.size(), 3u);
  EXPECT_EQ(cols[0], "Alice");
  EXPECT_EQ(cols[1], "30");
  EXPECT_EQ(cols[2], "NYC");

  // Third row - data
  reader.reset();
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols.size(), 3u);
  EXPECT_EQ(cols[0], "Bob");
  EXPECT_EQ(cols[1], "25");
  EXPECT_EQ(cols[2], "LA");
}

// Tests for EOF
TEST(CsvReaderTest, NoMoreRows) {
  CsvReader reader("test_data/sample.csv");
  EXPECT_TRUE(reader.is_open());

  std::vector<std::string> cols;

  // Read all rows
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_TRUE(reader.read_row(cols));

  // Next read should return false (EOF)
  EXPECT_FALSE(reader.read_row(cols));
}

// Tests for missing file
TEST(CsvReaderTest, MissingFile) {
  CsvReader reader("nonexistent_file.csv");
  EXPECT_FALSE(reader.is_open());
  std::vector<std::string> cols;
  EXPECT_FALSE(reader.read_row(cols));
}

// Tests for reset functionality
TEST(CsvReaderTest, Reset) {
  CsvReader reader("test_data/sample.csv");
  EXPECT_TRUE(reader.is_open());

  std::vector<std::string> cols;

  // Read header
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "name");

  // Read first data row
  reader.reset();
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "Alice");

  // Read second data row
  reader.reset();
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "Bob");

  // After reading all rows, reset should give us header again
  reader.reset();
  EXPECT_TRUE(reader.read_row(cols));
  EXPECT_EQ(cols[0], "name");
}
