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
  // TODO: data skip lines

  Progress ProgressBar = Progress();

  RtInfo rts(rt_info, var_pos_info.names());
  VarInfo vars(var_pos_info, rts.getNumRts());

  std::vector<ColumnPtr> out = createAllColumns(var_types, var_opts, &pEncoder_);
  resizeAllColumns(out, 10000); // Start out with 10k rows
  int i = 0;
  const char* line_start;
  const char* line_end;
  while (!data->isDone()) { // TODO: and less than n_max
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

