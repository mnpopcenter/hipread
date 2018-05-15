#include "varinfo.h"
#include <Rcpp.h>
using namespace Rcpp;

VarInfo::VarInfo(List var_pos_info, int num_rt) {
  for (int i = 0; i < num_rt; i++) {
    starts.push_back(as<List>(var_pos_info[i])["start"]);
    widths.push_back(as<List>(var_pos_info[i])["width"]);
    var_pos.push_back(as<List>(var_pos_info[i])["var_pos"]);
    num_vars_rectype.push_back(starts[i].size());
    max_ends.push_back(as<IntegerVector>(as<List>(var_pos_info[i])["max_end"])[0]);
  }
}

int VarInfo::get_start(int rt_index, int col_num) {
  return starts[rt_index][col_num];
}

int VarInfo::get_width(int rt_index, int col_num) {
  return widths[rt_index][col_num];
}

int VarInfo::get_var_pos(int rt_index, int col_num) {
  return var_pos[rt_index][col_num];
}

int VarInfo::get_num_vars(int rt_index) {
  return num_vars_rectype[rt_index];
}

int VarInfo::get_max_end(int rt_index) {
  return max_ends[rt_index];
}
