#include <fstream>
#include "column.h"
#include "Progress.h"
#include "datasource.h"
#include "varinfo.h"
#include "rtinfo.h"
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
    bool progress
) {
  const int PROGRESS_TICK = 16384;
  List rt_info = as<List>(rt_info_);
  List var_pos_info = as<List>(var_pos_info_);
  List var_opts = as<List>(var_opts_);

  DataSourcePtr data = newDataSource(as<std::string>(filename[0]), isGzipped);

  Progress ProgressBar = Progress();

  RtInfo rts(rt_info, var_pos_info.names());
  VarInfo vars(var_pos_info, rts.getNumRts());

  std::vector<ColumnPtr> out = createAllColumns(var_types);
  resizeAllColumns(out, 10000); // Start out with 10k rows
  int i = 0;
  while (!data->isDone()) {
    if (i >= out[0]->size()) {
      // Resize by guessing from the progress bar
      resizeAllColumns(out, (i / data->progress_info().first) * 1.1);
    }
    std::string line;
    data->getLine(line);

    int rt_index = rts.getRtIndex(line);
    if (rt_index < 0) {
      // TODO: Should this be a warning?
      continue;
    }

    // Check if raw line is long enough
    if ((int) line.length() < vars.get_max_end(rt_index)) {
      Rcpp::stop("Line is too short for rectype.");
    }

    // Loop through vars in rectype and add to out
    for (int j = 0; j < vars.get_num_vars(rt_index); j++) {
      std::string x = line.substr(
        vars.get_start(rt_index, j),
        vars.get_width(rt_index, j)
      );
      int cur_var_pos = vars.get_var_pos(rt_index, j);

      out[cur_var_pos]->setValue(i, x, var_opts[cur_var_pos]);
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

