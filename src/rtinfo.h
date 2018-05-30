#ifndef HIPREAD_RTINFO_H_
#define HIPREAD_RTINFO_H_

#include <Rcpp.h>
using namespace Rcpp;

class RtInfo {
private:
  int start;
  int width;
  std::vector<std::string> rectypes;
  bool hierarchical;

public:
  RtInfo(List rt_info, std::vector<std::string> rectypes_);
  int getRtIndex(const char* line_start, const char* line_end);
  int getNumRts();
};

#endif
