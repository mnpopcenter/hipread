#include <Rcpp.h>
using namespace Rcpp;
#include <zlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "gzstream.h"


void GzStream::fillBuffer() {
  int err;
  char* offset;
  if (cur != NULL) {
    offset = std::copy(cur, end, buffer);
  } else {
    offset = buffer;
  }

  int len = sizeof(buffer) - (offset - buffer);
  if (len <= 0) stop("Line length too long; cannot read file.");

  len = gzread(file, offset, len);

  if (len < 0) stop(gzerror(file, &err));

  cur = buffer;
  end = offset + len;
}

bool GzStream::getLine(const char* &line_start, const char* &line_end) {
  if (isDone()) return false;
  char* eol = std::find(cur, end, '\n');

  if (eol >= end) {
    if (gzeof(file)) {
      done = true;
      line_start = cur;
      line_end = end;
      cur = end;
      if (gzclose(file) != Z_OK) stop("Could not close file");
      return true;
    } else {
      fillBuffer();
      eol = std::find(cur, end, '\n');
      if (eol >= end) stop("Line length too long; cannot read file.");
    }
  }
  line_start = cur;
  line_end = eol;
  total_read_ += (eol - cur);
  cur = eol + 1;
  return true;
}

bool GzStream::isDone() {
  return done && cur == end;
}

size_t GzStream::getTotalSizeEstimate() {
  std::ifstream raw_(filename_);
  raw_.seekg(0, std::ifstream::end);
  size_t total_file_bytes = raw_.tellg();

  return total_file_bytes;
}

size_t GzStream::getProgress() {
  size_t out = gzoffset(file);
  return out;
}
