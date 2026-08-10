/**
 * @file csv_reader.hpp
 * @brief CSV file reader class
 */

#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include <string>
#include <fstream>
#include <vector>

class CsvReader {
public:
  explicit CsvReader(const std::string& path);
  
  bool is_open() const;
  bool read_row(std::vector<std::string>& cols);
  void reset();

private:
  std::ifstream m_file;
  bool m_open;
  std::string m_current_line;
  int m_row_count;
};

#endif // CSV_READER_HPP
