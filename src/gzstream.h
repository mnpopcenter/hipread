#ifndef HIPREAD_GZSTREAM_H_
#define HIPREAD_GZSTREAM_H_

#include <Rcpp.h>
using namespace Rcpp;
#include <zlib.h>
#include <iostream>
#include <algorithm>

// Adapted from https://stackoverflow.com/questions/21426427/handling-large-gzfile-in-c
// I wish I could use the boost libraries but this makes me think that's not so easy:
// https://github.com/eddelbuettel/bh/issues/49

// This number has to be longer than all line lengths
static const unsigned BUFLEN = 1048576;

class GzStream {
private:
  std::string filename_;
  gzFile file;
  char buffer[BUFLEN];
  char* offset;
  char* cur;
  char* end;
  size_t total_read_;
  void fillBuffer();

public:
  GzStream(std::string filename) : filename_(filename), total_read_(0) {
    offset = buffer;
    file = gzopen(filename.c_str(), "rb");
    fillBuffer();
  };
  ~GzStream() {
    offset = NULL;
    cur = NULL;
    end = NULL;
  }
  bool getLine(std::string &line);
  bool isDone();
  size_t getTotalSizeEstimate();
  size_t getProgress();
};
#endif
