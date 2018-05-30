#include <Rcpp.h>
using namespace Rcpp;

#include "rtinfo.h"

RtInfo::RtInfo(List rt_info, std::vector<std::string> rectypes_) : rectypes(rectypes_) {
  start = as<int>(rt_info["start"]);
  width = as<int>(rt_info["width"]);
  hierarchical = width > 0;
}

int RtInfo::getRtIndex(const char* line_start, const char* line_end) {
  if (!hierarchical) return 0;

  std::string rt(line_start + start, line_start + start + width);

  int rt_index = std::distance(
    rectypes.begin(),
    std::find(rectypes.begin(), rectypes.end(), rt)
  );

  if (rt_index == rectypes.size()) rt_index = -1;
  return rt_index;
}

int RtInfo::getNumRts() {
  return rectypes.size();
}
