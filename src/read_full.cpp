#include <fstream>
#include "column.h"
#include "Progress.h"
#include "datasource.h"
#include "varinfo.h"
#include "rtinfo.h"
#include "iconv.h"
#include <algorithm>
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
RObject read_long(
    CharacterVector filename,
    CharacterVector var_names,
    CharacterVector var_types,
    List rt_info_,
    List var_pos_info_,
    List var_opts_,
    int skip,
    int n_max,
    bool isGzipped,
    CharacterVector encoding,
    bool progress
) {
  const int PROGRESS_TICK = 16384;
  List rt_info = as<List>(rt_info_);
  List var_pos_info = as<List>(var_pos_info_);
  List var_opts = as<List>(var_opts_);
  Iconv pEncoder_(as<std::string>(encoding));

  DataSourcePtr data = newDataSource(as<std::string>(filename[0]), isGzipped);
  data->skipLines(skip);

  Progress ProgressBar = Progress();

  RtInfo rts(rt_info, var_pos_info.names());
  VarInfo vars(var_pos_info, rts.getNumRts());

  std::vector<ColumnPtr> out = createAllColumns(var_types, var_opts, &pEncoder_);
  resizeAllColumns(out, 10000); // Start out with 10k rows
  int i = 0;
  const char* line_start;
  const char* line_end;
  while (!data->isDone() && i < n_max) {
    data->getLine(line_start, line_end);

    if (line_end - line_start == 0 && data->isDone()) {
      break;
    }

    if (i >= out[0]->size()) {
      // Resize by guessing from the progress bar
      resizeAllColumns(out, static_cast<int>((i / data->progress_info().first) * 1.1));
    }

    size_t rt_index;
    bool rt_found = rts.getRtIndex(line_start, line_end, rt_index);
    if (!rt_found) {
      // TODO: Should this be a warning?
      continue;
    }

    // Check if raw line is long enough
    if (line_end - line_start < vars.get_max_end(rt_index)) {
      Rcpp::stop("Line is too short for rectype.");
    }

    // Loop through vars in rectype and add to out
    for (size_t j = 0; j < vars.get_num_vars(rt_index); j++) {
      const char *x_start = line_start + vars.get_start(rt_index, j);
      const char *x_end = x_start + vars.get_width(rt_index, j);

      size_t cur_var_pos = vars.get_var_pos(rt_index, j);

      out[cur_var_pos]->setValue(i, x_start, x_end);
    }

    if (progress && i % PROGRESS_TICK == 0) {
      ProgressBar.show(data->progress_info());
    }
    ++i;
  }
  resizeAllColumns(out, i);
  ProgressBar.stop();
  return columnsToDf(out, var_names);
}

// [[Rcpp::export]]
RObject read_list(
    CharacterVector filename,
    List var_names_,
    List var_types_,
    List rt_info_,
    List var_pos_info_,
    List var_opts_,
    int skip,
    int n_max,
    bool isGzipped,
    CharacterVector encoding,
    bool progress
) {
  const int PROGRESS_TICK = 16384;

  List var_names = as<List>(var_names_);
  List var_types = as<List>(var_types_);
  List rt_info = as<List>(rt_info_);
  List var_pos_info = as<List>(var_pos_info_);
  List var_opts = as<List>(var_opts_);
  Iconv pEncoder_(as<std::string>(encoding));

  DataSourcePtr data = newDataSource(as<std::string>(filename[0]), isGzipped);
  data->skipLines(skip);

  Progress ProgressBar = Progress();

  RtInfo rts(rt_info, var_pos_info.names());
  VarInfo vars(var_pos_info, rts.getNumRts());

  std::vector<std::vector<ColumnPtr> > out;
  std::vector<int> cur_pos_rt;
  for (size_t i = 0; i < rts.getNumRts(); ++i) {
    Rcpp::CharacterVector vt = var_types[static_cast<R_xlen_t>(i)];
    out.push_back(createAllColumns(vt, var_opts[static_cast<R_xlen_t>(i)], &pEncoder_));
    resizeAllColumns(out[i], 10000); // Start out with 10k rows
    cur_pos_rt.push_back(-1);
  }

  int i = 0;
  const char* line_start;
  const char* line_end;
  while (!data->isDone() && i < n_max) {
    data->getLine(line_start, line_end);

    if (line_end - line_start == 0 && data->isDone()) {
      break;
    }

    size_t rt_index;
    bool rt_found = rts.getRtIndex(line_start, line_end, rt_index);
    if (!rt_found) {
      // TODO: Should this be a warning?
      continue;
    }
    cur_pos_rt[rt_index]++;

    if (cur_pos_rt[rt_index] >= out[rt_index][0]->size()) {
      // Resize by guessing from the progress bar
      resizeAllColumns(out[rt_index], static_cast<int>((cur_pos_rt[rt_found] / data->progress_info().first) * 1.1));
    }

    // Check if raw line is long enough
    if (line_end - line_start < vars.get_max_end(rt_index)) {
      Rcpp::stop("Line is too short for rectype.");
    }

    // Loop through vars in rectype and add to out
    for (size_t j = 0; j < vars.get_num_vars(rt_index); j++) {
      const char *x_start = line_start + vars.get_start(rt_index, j);
      const char *x_end = x_start + vars.get_width(rt_index, j);

      out[rt_index][j]->setValue(cur_pos_rt[rt_index], x_start, x_end);
    }

    if (progress && i % PROGRESS_TICK == 0) {
      ProgressBar.show(data->progress_info());
    }
    ++i;
  }

  List out_r;
  for (size_t j = 0;  j < rts.getNumRts(); ++j) {
    resizeAllColumns(out[j], cur_pos_rt[j] + 1);
    out_r.push_back(columnsToDf(out[j], var_names[static_cast<R_xlen_t>(j)]));
  }
  out_r.names() = var_pos_info.names();

  ProgressBar.stop();
  return out_r;
}
