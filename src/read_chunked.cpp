#include <fstream>
#include "column.h"
#include "Progress.h"
#include "datasource.h"
#include "varinfo.h"
#include "rtinfo.h"
#include <algorithm>
#include <Rcpp.h>
using namespace Rcpp;


Function R6method(Environment env, const std::string& method) {
  return as<Function>(env[method]);
}
bool isTrue(SEXP x) {
  if (!(TYPEOF(x) == LGLSXP && Rf_length(x) == 1)) {
    stop("`continue()` must return a length 1 logical vector");
  }
  return LOGICAL(x)[0] == TRUE;
}

// https://stackoverflow.com/questions/25288604/how-to-read-non-ascii-lines-from-file-with-stdifstream-on-linux

// [[Rcpp::export]]
void read_chunked_long(
    CharacterVector filename,
    Environment callback,
    NumericVector chunksize,
    CharacterVector var_names,
    CharacterVector var_types,
    List rt_info_,
    List var_pos_info_,
    List var_opts_,
    bool isGzipped,
    bool progress
) {
  List rt_info = as<List>(rt_info_);
  List var_pos_info = as<List>(var_pos_info_);
  List var_opts = as<List>(var_opts_);

  DataSourcePtr data = newDataSource(as<std::string>(filename[0]), isGzipped);

  Progress ProgressBar = Progress();

  RtInfo rts(rt_info, var_pos_info.names());
  VarInfo vars(var_pos_info, rts.getNumRts());

  while (isTrue(R6method(callback, "continue")()) && !data->isDone()) {
    std::vector<ColumnPtr> chunk = createAllColumns(var_types, var_opts);
    resizeAllColumns(chunk, chunksize[0]);

    int i;
    for (i = 0; i < chunksize[0] - 1; ++i) {
      std::string line;
      data->getLine(line);

      if (line.length() == 0 && data->isDone()) {
        break;
      }

      int rt_index = rts.getRtIndex(line);
      if (rt_index < 0) {
        // TODO: Should this be a warning?
        break;
      }

      // Check if raw line is long enough
      if ((int) line.length() < vars.get_max_end(rt_index)) {
        Rcpp::stop("Line is too short for rectype.");
      }

      // Loop through vars in rectype and add to out
      for (int j = 0; j < vars.get_num_vars(rt_index); j++) {
        const char *x_start = line.c_str() + vars.get_start(rt_index, j);
        const char *x_end = x_start + vars.get_width(rt_index, j);

        int cur_var_pos = vars.get_var_pos(rt_index, j);

        chunk[cur_var_pos]->setValue(i, x_start, x_end);
      }
    }

    resizeAllColumns(chunk, i);
    RObject chunk_df = columnsToDf(chunk, var_names);
    R6method(callback, "receive")(chunk_df, i);

    if (progress) {
      ProgressBar.show(data->progress_info());
    }
  }
  ProgressBar.stop();
}

