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
  char* cur;
  char* end;
  size_t total_read_;
  void fillBuffer();
  bool done;

public:
  GzStream(std::string filename) : filename_(filename), total_read_(0), done(false) {
    cur = NULL;
    file = gzopen(filename.c_str(), "rb");
    fillBuffer();
  };
  ~GzStream() {
    cur = NULL;
    end = NULL;
  }
  bool getLine(const char* &line_start, const char* &line_end);
  bool isDone();
  size_t getTotalSizeEstimate();
  size_t getProgress();
};
#endif
