#' Calculate frequencies from fixed width file without loading into memory
#'
#' Calculate the frequency of values in all variables in a fixed width file.
#' Does so without holding the whole data in memory or creating a full
#' R data.frame and calling R code on interim pieces. (Probably only
#' useful inside IPUMS HQ).
#'
#' @inheritParams readh_long
#'
#' @return A list of frequencies
#' @export
#' @keywords internal
readh_freqs <- function(
  file, var_names, rt_start, rt_width,
  var_pos_info, compression = NULL, progress = show_progress()
) {
  check_file(file)
  isgzipped <- is_gzip_compression(compression, file)
  rtinfo <- create_rt_info(rt_start, rt_width)
  var_pos_info <- check_long_var_pos_info(var_pos_info)
  check_freq_args(var_names, var_pos_info)

  read_freqs(
    file, var_names, rtinfo, var_pos_info, isgzipped, progress
  )
}
