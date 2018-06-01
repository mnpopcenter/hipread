#include <Rcpp.h>
using namespace Rcpp;
#include <fstream>
#include <iostream>
#include <zlib.h>
#include <algorithm>
#include "boost.h"
#include "datasource.h"

// This should never be called, but I include it here to avoid
// -Weverything warning about no out-of-line virtual class defintions
void DataSource::getLine(const char* &start, const char* &end) {
  start = nullptr;
  end = nullptr;
}

void FileDataSource::getLine(const char* &start, const char* &end) {
  if (cur_end != nullptr) cur_begin = cur_end + 1;
  cur_end = std::find_if(cur_begin, file_end, [](int ch) {
    return ch == '\n';
  });
  if (cur_end > file_end) cur_end = file_end;

  start = cur_begin;
  end = cur_end;
}

bool FileDataSource::isDone() {
  return cur_begin == file_end;
}

std::pair<double, size_t> FileDataSource::progress_info() {
  if (isDone()) {
    return std::make_pair(1.0, total_size_);
  } else {
    size_t current = static_cast<size_t>(cur_begin - file_begin);
    return std::make_pair(current / static_cast<double>(total_size_), current);
  }
}


size_t GzFileDataSource::get_size() {
  return data_->getTotalSizeEstimate();
}


void GzFileDataSource::getLine(const char* &start, const char* &end) {
  data_->getLine(start, end);
}

bool GzFileDataSource::isDone() {
  return data_->isDone();
}

std::pair<double, size_t> GzFileDataSource::progress_info() {
  if (isDone()) {
    return std::make_pair(1.0, total_size_);
  } else {
    size_t current = data_->getProgress() ;
    return std::make_pair(current / static_cast<double>(total_size_), current);
  }
}

DataSourcePtr newDataSource(std::string filename, bool isCompressed) {
  if (isCompressed) {
    return DataSourcePtr(new GzFileDataSource(filename));
  } else {
    return DataSourcePtr(new FileDataSource(filename));
  }
}
