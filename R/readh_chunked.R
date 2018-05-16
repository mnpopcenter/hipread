#' Read a hierarchical data file to long format, in chunks
#'
#' Analagous to `readr::read_fwf()`, but with chunks, and allowing for
#' hierarchical fixed width data files (where the data file has rows of
#' different record types, each with their own variables and column
#' specifications). Reads the data into "long" format, meaning that
#' there is one row per observation, and variables that don't apply to
#' the current observation receive missing values.
#'
#' @inheritParams readh_long
#' @param callback A `callback()` function, allowing you to perform a
#'   function on each chunk.
#' @param chunk_size The size of the chunks that will be read as a
#'   single unit (defaults to 10,000)
#'
#' @return Depends on the type of `callback` function you use
#' @export
#'
#' @examples
#'
readh_long_chunked <- function(
  file, callback, chunk_size, var_names, var_types, rt_start, rt_width,
  var_pos_info, var_opts, compression = NULL, progress = show_progress()
) {
  check_file(file)
  isgzipped <- is_gzip_compression(compression, file)
  rtinfo <- create_rt_info(rt_start, rt_width)
  var_pos_info <- check_long_var_pos_info(var_pos_info)
  check_long_arg_lengths(var_names, var_types, var_pos_info, var_opts)
  var_opts <- check_var_opts(var_opts, var_types)


  callback <- as_chunk_callback(callback)
  on.exit(callback$finally(), add = TRUE)

  read_chunked_long(
    file, callback, chunk_size, var_names, var_types, rtinfo,
    var_pos_info, var_opts, isgzipped, progress
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
