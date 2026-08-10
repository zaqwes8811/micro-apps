/**
 * @file csv_reader.cpp
 * @brief Implementation of CsvReader class
 */

#include "csv_reader.hpp"

CsvReader::CsvReader(const std::string& path)
    : m_file(path, std::ios::in), m_open(false), m_current_line(""), m_row_count(0) {
  if (m_file.is_open()) {
    m_open = true;
    // Clear file state
    m_file.clear();
    m_file.seekg(0, std::ios::beg);
    m_row_count = 0;
  }
}

bool CsvReader::is_open() const {
  return m_open;
}

bool CsvReader::read_row(std::vector<std::string>& cols) {
  if (!m_open) {
    return false;
  }

  // Get the next line
  if (!std::getline(m_file, m_current_line)) {
    return false;
  }

  // Skip empty lines
  if (m_current_line.empty()) {
    return false;
  }

  // Split line by commas
  cols.clear();
  size_t start = 0;
  size_t pos = m_current_line.find(',');

  while (pos != std::string::npos) {
    std::string col = m_current_line.substr(start, pos - start);
    // Trim trailing \r if present
    if (!col.empty() && col.back() == '\r') {
      col.pop_back();
    }
    cols.push_back(col);
    start = pos + 1;
    pos = m_current_line.find(',', start);
  }

  // Add the last column
  if (start < m_current_line.length()) {
    std::string col = m_current_line.substr(start);
    // Trim trailing \r if present
    if (!col.empty() && col.back() == '\r') {
      col.pop_back();
    }
    cols.push_back(col);
  }

  m_row_count++;
  return true;
}

void CsvReader::reset() {
  m_file.clear();
  m_file.seekg(0, std::ios::beg);
  m_current_line.clear();
  m_row_count = 0;
}
