#include <Rcpp.h>
using namespace Rcpp;
#include <fstream>
#include <iostream>
#include <zlib.h>
#include <algorithm>
#include "boost.h"
#include "datasource.h"


size_t FileDataSource::get_size() {
  data_->seekg(0, std::ifstream::end);
  size_t total_file_bytes = data_->tellg();
  data_->seekg(0, std::ifstream::beg);

  return total_file_bytes;
}

void FileDataSource::getLine(std::string &line) {
  std::getline(*data_, line);
}

bool FileDataSource::isDone() {
  return data_->eof();
}

std::pair<double, size_t> FileDataSource::progress_info() {
  if (isDone()) {
    return std::make_pair(1.0, total_size_);
  } else {
    size_t current = data_->tellg();
    return std::make_pair(current / (double)(total_size_), current);
  }
}


size_t GzFileDataSource::get_size() {
  return data_->getTotalSizeEstimate();
}


void GzFileDataSource::getLine(std::string &line) {
  data_->getLine(line);
}

bool GzFileDataSource::isDone() {
  return data_->isDone();
}

std::pair<double, size_t> GzFileDataSource::progress_info() {
  if (isDone()) {
    return std::make_pair(1.0, total_size_);
  } else {
    size_t current = data_->getProgress();
    if (current >= total_size_) {
      return std::make_pair((total_size_ - 1) / (double) total_size_, total_size_ - 1);
    } else {
      return std::make_pair(current / (double)(total_size_), current);
    }
  }
}

DataSourcePtr newDataSource(std::string filename, bool isCompressed) {
  if (isCompressed) {
    return DataSourcePtr(new GzFileDataSource(filename));
  } else {
    return DataSourcePtr(new FileDataSource(filename));
  }
};
