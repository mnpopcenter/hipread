# Adapted from readr
readh_long_chunked <- function(
  data, callback, chunk_size, var_names, var_types, rt_info_,
  var_pos_info_, var_opts_
) {
  callback <- as_chunk_callback(callback)
  on.exit(callback$finally(), add = TRUE)

  read_ipums_chunked_long(
    data, callback, chunk_size, var_names, var_types, rt_info_,
    var_pos_info_, var_opts_
  )

  return(callback$result())
}


as_chunk_callback <- function(x) UseMethod("as_chunk_callback")
as_chunk_callback.function <- function(x) {
  SideEffectChunkCallback$new(x)
}
as_chunk_callback.R6ClassGenerator <- function(x) {
  as_chunk_callback(x$new())
}
as_chunk_callback.ChunkCallback <- function(x) {
  x
}
