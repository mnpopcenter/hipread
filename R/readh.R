# Adapted from readr
readh_long <- function(
  data, var_names, var_types, rt_info_,
  var_pos_info_, var_opts_, progress = show_progress()
) {

  isgzipped <- tools::file_ext(data) == "gz"

  read_long(
    data, var_names, var_types, rt_info_,
    var_pos_info_, var_opts_, isgzipped, progress
  )
}
