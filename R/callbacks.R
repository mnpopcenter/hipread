# On some machines, I have seen R CMD Check warnings
# about not using dplyr even though the HipDataFrameCallBack
# uses it, and I hope this fixes that
bind_rows <- dplyr::bind_rows

#' Callback classes
#'
#' These classes are used to define callback behaviors, and are based
#' on readr's [`readr::callback`] functions.
#'
#' - The function `HipDataFrameCallback()` is similar to
#' [readr::DataFrameCallback()] except that it uses `dplyr::bind_rows()`
#' instead of `rbind()` so that it is faster.
#'
#'
#' @usage NULL
#' @format NULL
#' @name callback
#' @keywords internal
NULL

#' @usage NULL
#' @format NULL
#' @rdname callback
#' @export
HipDataFrameCallback <- R6::R6Class(
  "HipDataFrameCallback", inherit = readr::ChunkCallback,
  private = list(
    results = list()
  ),
  public = list(
    initialize = function(callback) {
      private$callback <- callback
    },
    receive = function(data, index) {
      result <- private$callback(data, index)
      private$results <- c(private$results, list(result))
    },
    result = function() {
      dplyr::bind_rows(private$results)
    },
    finally = function() {
      private$results <- list()
    }
  )
)
