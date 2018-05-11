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
  virtual void getLine(std::string &line) = 0;
  virtual bool isDone() = 0;
  virtual std::pair<double, size_t> progress_info() = 0;
};


class FileDataSource : public DataSource {
private:
  std::string filename_;
  size_t total_size_;
  std::ifstream* data_;
  size_t get_size();
public:
  FileDataSource(std::string filename) : DataSource(filename){
    data_ = new std::ifstream(filename);
    total_size_ = get_size();
  };
  ~FileDataSource() {
    if (data_) delete data_;
  }
  void getLine(std::string &line);
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
  void getLine(std::string &line);
  bool isDone();
  std::pair<double, size_t> progress_info();
};

DataSourcePtr newDataSource(std::string filename, bool isCompressed);

#endif
