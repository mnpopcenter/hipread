// Adapted from readr's source
#ifndef HIPREAD_DATASOURCE_H_
#define HIPREAD_DATASOURCE_H_

#include <Rcpp.h>
using namespace Rcpp;
#include <fstream>

class DataSource {
  std::string filename_;
public:
  DataSource(std::string filename) : filename_(filename){};
  virtual void getLine(std::string &line) = 0;
  virtual bool isDone() = 0;
  virtual std::pair<double, size_t> progress_info() = 0;
};


class FileDataSource : DataSource {
private:
  std::string filename_;
  size_t total_size_;
  std::ifstream data_;
  size_t get_size();
public:
  FileDataSource(std::string filename) : DataSource(filename){
    data_ = std::ifstream(filename);
    total_size_ = get_size();
  };
  void getLine(std::string &line);
  bool isDone();
  std::pair<double, size_t> progress_info();
};


#endif
