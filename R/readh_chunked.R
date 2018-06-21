#' Read a hierarchical data file to long format, in chunks
#'
#' Analagous to [readr::read_fwf()], but with chunks, and allowing for
#' hierarchical fixed width data files (where the data file has rows of
#' different record types, each with their own variables and column
#' specifications). Reads the data into "long" format, meaning that
#' there is one row per observation, and variables that don't apply to
#' the current observation receive missing values.
#'
#' @inheritParams hipread_long
#' @param callback A [`callback`] function, allowing you to perform a
#'   function on each chunk.
#' @param chunk_size The size of the chunks that will be read as a
#'   single unit (defaults to 10000)
#'
#' @return Depends on the type of [`callback`] function you use
#' @export
#'
#' @examples
#' # Read in a data, filtering out hhnum == "002"
#' data <- readh_long_chunked(
#'   hipread_example("test-basic.dat"),
#'   HipDataFrameCallback$new(function(x, pos) x[x$hhnum != "002", ]),
#'   4,
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
readh_long_chunked <- function(
  file, callback, chunk_size, var_names, var_types, rt_start, rt_width,
  var_pos_info, var_opts = NULL, skip = 0, encoding = "UTF-8", compression = NULL,
  progress = show_progress()
) {
  check_file(file)
  isgzipped <- is_gzip_compression(compression, file)
  rtinfo <- create_rt_info(rt_start, rt_width)
  var_pos_info <- check_long_var_pos_info(var_pos_info)
  var_opts <- check_var_opts(var_opts, var_types, var_names)
  check_long_arg_lengths(var_names, var_types, var_pos_info, var_opts)
  skip <- check_skip(skip)

  callback <- as_chunk_callback(callback)
  on.exit(callback$finally(), add = TRUE)

  read_chunked_long(
    file, callback, chunk_size, var_names, var_types, rtinfo,
    var_pos_info, var_opts, skip, isgzipped, encoding, progress
  )

  return(callback$result())
}

# Copied from readr
as_chunk_callback <- function(x) UseMethod("as_chunk_callback")
as_chunk_callback.function <- function(x) {
  readr::SideEffectChunkCallback$new(x)
}
as_chunk_callback.R6ClassGenerator <- function(x) {
  as_chunk_callback(x$new())
}
as_chunk_callback.ChunkCallback <- function(x) {
  x
}
