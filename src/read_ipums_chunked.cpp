#include <fstream>
// #include <boost/iostreams/filtering_stream.hpp>
// #include <boost/iostreams/filter/gzip.hpp>
#include "column.h"

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
void read_ipums_chunked_long(
    CharacterVector filename,
    Environment callback,
    NumericVector chunksize,
    CharacterVector var_names,
    CharacterVector var_types,
    List rt_info_,
    List var_pos_info_,
    List var_opts_
) {
  List rt_info = as<List>(rt_info_);
  List var_pos_info = as<List>(var_pos_info_);
  List var_opts = as<List>(var_opts_);

  std::ifstream filein(filename[0]);

  int rt_start = as<int>(rt_info["start"]);
  int rt_width = as<int>(rt_info["width"]);

  std::vector<std::string> rectypes = var_pos_info.names();
  int num_rt = rectypes.size();

  std::vector<std::vector<int> > starts;
  std::vector<std::vector<int> > widths;
  std::vector<std::vector<int> > var_pos;
  std::vector<int> num_vars_rectype;
  std::vector<int> max_ends;
  for (int i = 0; i < num_rt; i++) {
    starts.push_back(as<List>(var_pos_info[i])["start"]);
    widths.push_back(as<List>(var_pos_info[i])["width"]);
    var_pos.push_back(as<List>(var_pos_info[i])["var_pos"]);
    num_vars_rectype.push_back(starts[i].size());
    max_ends.push_back(as<IntegerVector>(as<List>(var_pos_info[i])["max_end"])[0]);
  }


  while (isTrue(R6method(callback, "continue")()) && !filein.eof()) {
    std::vector<ColumnPtr> chunk = createAllColumns(var_types);
    resizeAllColumns(chunk, chunksize[0]);

    int i = 0;
    for (std::string line; std::getline(filein, line);) {
      std::string rt = line.substr(rt_start, rt_width);

      int rt_index = -1;
      for (int j = 0; j < num_rt; j++) {
        if (rt == rectypes[j]) {
          rt_index = j;
          break;
        }
      }

      if (rt_index == -1) {
        break;
      }

      // Check if raw line is long enough
      if (line.length() < max_ends[rt_index]) {
        Rcpp::stop("Line is too short for rectype.");
      }

      // Loop through vars in rectype and add to out
      for (int j = 0; j < num_vars_rectype[rt_index]; j++) {
        std::string x = line.substr(starts[rt_index][j], widths[rt_index][j]);
        int cur_var_pos = var_pos[rt_index][j];

        chunk[cur_var_pos]->setValue(i, x, var_opts[cur_var_pos]);
      }

      if (++i > chunksize[0] - 1) break;
    }
    resizeAllColumns(chunk, i);

    RObject chunk_df = columnsToDf(chunk, var_names);

    R6method(callback, "receive")(chunk_df, i);
  }
}

