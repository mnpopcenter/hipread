#include <Rcpp.h>
using namespace Rcpp;
#include <zlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "gzstream.h"


void GzStream::fillBuffer() {
  int err;

  if (offset != buffer) {
    offset = std::copy(cur, end, buffer);
  }

  int len = sizeof(buffer) - (offset - buffer);
  if (len == 0) stop("Line length too long; cannot read file.");

  len = gzread(file, offset, len);

  if (len == 0) return;
  if (len < 0) stop(gzerror(file, &err));

  cur = buffer;
  end = offset + len;
}

bool GzStream::getLine(std::string &line) {
  if (isDone()) return false;
  char* eol = std::find(cur, end, '\n');

  if (eol >= end) {
    if (gzeof(file)) {
      done = true;
      line = std::string(cur, end);
      // std::copy(cur, end, line); // TODO: I think this is the source of current compiler error
      cur = end;
      if (gzclose(file) != Z_OK) stop("Could not close file");
      return true;
    } else {
      fillBuffer();
      eol = std::find(cur, end, '\n');
    }
  }

  // std::copy(cur, eol, line);
  line = std::string(cur, eol);
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
