/**
 * @file csv_reader.cpp
 * @brief Implementation of CsvReader class
 */

#include "csv_reader.hpp"

CsvReader::CsvReader(const std::string& path, bool has_header)
    : m_path(path), m_file(path, std::ios::in), m_open(false), m_has_header(has_header), m_header_skipped(false), m_current_line(""), m_row_count(0) {
  if (m_file.is_open()) {
    m_open = true;
    // Clear file state
    m_file.clear();
    m_file.seekg(0, std::ios::beg);
    m_row_count = 0;
    // Skip header if has_header is true
    if (m_has_header) {
      std::string dummy;
      std::getline(m_file, dummy); // Skip the header line
      m_header_skipped = true;
    }
  }
}

bool CsvReader::is_open() const {
  return m_open;
}

bool CsvReader::read_row(std::vector<std::string>& cols) {
  if (!m_open) {
    return false;
  }

  // Skip header if has_header is true and header not yet skipped
  if (m_has_header && !m_header_skipped) {
    m_header_skipped = true;
    std::string dummy;
    std::getline(m_file, dummy); // Skip the header line
    return false; // Return false because we're still in the process of skipping
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
  // Clear state and re-open file
  m_file.close();
  m_file = std::ifstream(m_path, std::ios::in);
  m_open = m_file.is_open();
  if (m_open) {
    m_file.clear();
    m_file.seekg(0, std::ios::beg);
  }
  m_header_skipped = false;
  m_row_count = 0;
}
