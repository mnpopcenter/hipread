#ifndef HIPREAD_GZSTREAM_H_
#define HIPREAD_GZSTREAM_H_

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
  void fillBuffer();
  bool done;

public:
  GzStream(std::string filename) : filename_(filename), done(false) {
    cur = nullptr;
    file = gzopen(filename.c_str(), "rb");
    fillBuffer();
  }
  ~GzStream() {
    cur = nullptr;
    end = nullptr;
  }
  bool getLine(const char* &line_start, const char* &line_end);
  bool isDone();
  size_t getTotalSizeEstimate();
  size_t getProgress();
  void skipBOM();
};
#endif
