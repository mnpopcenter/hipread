#include <Rcpp.h>
using namespace Rcpp;

#include "datasource.h"
#include <fstream>

size_t FileDataSource::get_size() {
  data_.seekg(0, std::ifstream::end);
  size_t total_file_bytes = data_.tellg();
  data_.seekg(0, std::ifstream::beg);

  return total_file_bytes;
}

void FileDataSource::getLine(std::string &line) {
  std::getline(data_, line);
}

bool FileDataSource::isDone() {
  return data_.eof();
}

std::pair<double, size_t> FileDataSource::progress_info() {
  if (isDone()) {
    return std::make_pair(1.0, total_size_);
  } else {
    size_t current = data_.tellg();
    return std::make_pair(current / (double)(total_size_), current);
  }
}
