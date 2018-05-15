#ifndef HIPREAD_VARINFO_H_
#define HIPREAD_VARINFO_H_

#include <Rcpp.h>
using namespace Rcpp;

class VarInfo {
private:
  std::vector<std::vector<int> > starts;
  std::vector<std::vector<int> > widths;
  std::vector<std::vector<int> > var_pos;
  std::vector<int> num_vars_rectype;
  std::vector<int> max_ends;

public:
  VarInfo(List var_pos_info, int num_rt);
  int get_start(int rt_index, int col_num);
  int get_width(int rt_index, int col_num);
  int get_var_pos(int rt_index, int col_num);
  int get_num_vars(int rt_index);
  int get_max_end(int rt_index);
};

#endif
