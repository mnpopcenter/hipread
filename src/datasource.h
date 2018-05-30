// Adapted from readr's source
#ifndef HIPREAD_DATASOURCE_H_
#define HIPREAD_DATASOURCE_H_

#include <Rcpp.h>
using namespace Rcpp;
#include <fstream>
#include <iostream>
#include "boost.h"
#include "gzstream.h"

class DataSource;
typedef boost::shared_ptr<DataSource> DataSourcePtr;


class DataSource {
  std::string filename_;
public:
  DataSource(std::string filename) : filename_(filename){};
  virtual ~DataSource(){};
  virtual void getLine(const char* &start, const char* &end) = 0;
  virtual bool isDone() = 0;
  virtual std::pair<double, size_t> progress_info() = 0;
};


class FileDataSource : public DataSource {
private:
  std::string filename_;
  size_t total_size_;
  boost::interprocess::file_mapping fm_;
  boost::interprocess::mapped_region mr_;
  char* file_begin;
  char* file_end;
  char* cur_begin;
  char* cur_end;

public:
  FileDataSource(std::string filename) : DataSource(filename){
    try {
      fm_ = boost::interprocess::file_mapping(
        filename.c_str(), boost::interprocess::read_only);
      mr_ = boost::interprocess::mapped_region(
        fm_, boost::interprocess::read_private);
    } catch (boost::interprocess::interprocess_exception& e) {
      Rcpp::stop("Cannot read file %s: %s", filename, e.what());
    }
    total_size_ = mr_.get_size();
    file_begin = static_cast<char*>(mr_.get_address());
    file_end = file_begin + total_size_;
    cur_begin = file_begin;
    cur_end = NULL;
  };
  ~FileDataSource() {
    file_end = NULL;
    file_begin = NULL;
    cur_begin = NULL;
    cur_end = NULL;
  }
  void getLine(const char* &start, const char* &end);
  bool isDone();
  std::pair<double, size_t> progress_info();
};



class GzFileDataSource : public DataSource {
private:
  std::string filename_;
  size_t total_size_;
  GzStream *data_;
  size_t get_size();
public:
  GzFileDataSource(std::string filename) : DataSource(filename) {
    data_ = new GzStream(filename);
    total_size_ = get_size();
  };
  ~GzFileDataSource() {
    if (data_) delete data_;
  }
  void getLine(const char* &start, const char* &end);
  bool isDone();
  std::pair<double, size_t> progress_info();
};

DataSourcePtr newDataSource(std::string filename, bool isCompressed);

#endif
