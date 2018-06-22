#' Read a hierarchical data file to long format
#'
#' Analagous to [readr::read_fwf()] but allowing for
#' hierarchical fixed width data files (where the data file has rows of
#' different record types, each with their own variables and column
#' specifications). Reads the data into "long" format, meaning that
#' there is one row per observation, and variables that don't apply to
#' the current observation receive missing values.
#'
#' @param file A filename
#' @param var_names A character vector of names for the variables
#' @param var_types A character vector of types for the variables. Allowed
#'   types are `"character"`, `"double"` and `"integer"`.
#' @param rt_start An integer indicating the start position of the rectype
#'   variable (1 indicates the first character)
#' @param rt_width An integer indicating the width of the rectype variable,
#'   if there's only one record type, this width can be 0.
#' @param var_pos_info A list of fixed width file definitions by rectype,
#'   if there's only one rectype, no name is required, otherwise the name
#'   indicates the value for the rectype variable that indicates the row
#'   is this rectype. Each of these lists must contain a list with equal
#'   length vectors of `start` to indicate the starting positions starting
#'    at 1, `width` to indicate the widths, and `var_pos` which indicates
#'    what order in the data.frame the variable is located (again starting
#'    at 1).
#' @param var_opts A vector of lists with variable specific options. For
#'   string variables, a logical indicator named `trim_ws` is allowed
#'   (if missing it defaults to `TRUE`) and for double variables an
#'   integer named `imp_dec` indicating the number of implicit decimals
#'   in the variable (defaults to 0).
#' @param skip Number of lines to skip at the start of the data (defaluts to 0).
#' @param n_max Maximum number of lines to read. Negative numbers (the default)
#'   reads all lines.
#' @param encoding (Defaults to UTF-8) A string indicating what encoding to use
#'   when reading the data, but like readr, the data will always be converted to
#'   UTF-8 once it is imported. Note that UTF-16 and UTF-32 are not supported for
#'   non-character columns.
#' @param compression If `NULL`, guesses the compression from the
#'   file extension (if extension is "gz" uses gzip, otherwise
#'   treats as plain text), can specify it with a string ("txt"
#'   indicates plain text and "gz" for gzip).
#' @param progress A logical indicating whether progress should be
#'   displayed on the screen, defaults to showing progress unless
#'   the current context is non-interactive or in a knitr document or
#'   if the user has turned off readr's progress by default using
#'   the option `options("readr.show_progress")`.
#'
#' @return A `tbl_df` data frame`
#' @export
#'
#' @examples
#' # Read an example data.frame
#' data <- hipread_long(
#'   hipread_example("test-basic.dat"),
#'   list(
#'     H = hip_fwf_positions(
#'       c(1, 2, 5, 8),
#'       c(1, 4, 7, 10),
#'       c("rt", "hhnum", "hh_char", "hh_dbl"),
#'       c("c", "i", "c", "d")
#'     ),
#'     P = hip_fwf_widths(
#'       c(1, 3, 1, 3, 1),
#'       c("rt", "hhnum",  "pernum", "per_dbl", "per_mix"),
#'       c("c", "i", "i", "d", "c")
#'     )
#'   ),
#'   1,
#'   1
#' )
hipread_long <- function(
  file, var_info, rt_start = 1, rt_width = 0, compression = NULL,
  skip = 0, n_max = -1, encoding = "UTF-8", progress = show_progress()
) {
  check_file(file)
  isgzipped <- is_gzip_compression(compression, file)
  rtinfo <- create_rt_info(rt_start, rt_width)
  var_info <- add_level_to_rect(var_info)
  var_names <- get_var_names(var_info)
  var_pos_info <- get_var_pos(var_info, var_names)
  var_types <- get_var_types(var_info, var_names)
  var_opts <- get_var_opts(var_info, var_names)
  skip <- check_skip(skip)
  n_max <- check_n_max(n_max)

  read_long(
    file, var_names, var_types, rtinfo, var_pos_info,
    var_opts, skip, n_max, isgzipped, encoding, progress
  )
}
