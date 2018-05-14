#' Callback classes
#'
#' These classes are used to define callback behaviors.
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
