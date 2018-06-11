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
#' data <- readh_long(
#'   hipread_example("test-basic.dat"),
#'   c("rt", "hhnum", "hh_char", "hh_dbl", "hh_impdbl", "pernum", "per_dbl", "per_mix"),
#'   c("character", "character", "character", "double", "double", "integer", "double", "character"),
#'   1,
#'   1,
#'   list(
#'     H = list(
#'       start = c(1, 2, 5, 8, 11),
#'       width = c(1, 3, 3, 3, 2),
#'       var_pos = c(1, 2, 3, 4, 5)
#'     ),
#'     P = list(
#'       start = c(1, 2, 5, 6, 9),
#'       width = c(1, 3, 1, 3, 1),
#'       var_pos = c(1, 2, 6, 7, 8)
#'     )
#'   ),
#'   list(
#'     list(trim_ws = TRUE),
#'     list(trim_ws = FALSE),
#'     list(trim_ws = TRUE),
#'     list(imp_dec = 0L),
#'     list(imp_dec = 1L),
#'     list(),
#'     list(imp_dec = 0L),
#'     list(trim_ws = TRUE)
#'   )
#' )

readh_long <- function(
  file, var_names, var_types, rt_start, rt_width,
  var_pos_info, var_opts, compression = NULL, progress = show_progress()
) {
  check_file(file)
  isgzipped <- is_gzip_compression(compression, file)
  rtinfo <- create_rt_info(rt_start, rt_width)
  var_pos_info <- check_long_var_pos_info(var_pos_info)
  check_long_arg_lengths(var_names, var_types, var_pos_info, var_opts)
  var_opts <- check_var_opts(var_opts, var_types)

  read_long(
    file, var_names, var_types, rtinfo,
    var_pos_info, var_opts, isgzipped, progress
  )
}
