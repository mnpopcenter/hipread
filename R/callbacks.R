# On some machines, I have seen R CMD Check warnings
# about not using dplyr even though the HipDataFrameCallBack
# uses it, and I hope this fixes that
bind_rows <- dplyr::bind_rows

#' Callback classes
#'
#' These classes are used to define callback behaviors, and are based
#' on readr's [`readr::callback`] functions.
#'
#' - The callbacks `HipChunkCallback`, `HipListCallback` and
#' `HipSideEffectChunkCallback` should be identical to their readr
#' counterparts, but have been copied into hipread to ensure that they
#' work even if readr changes.
#'
#' - The callback `HipDataFrameCallback` is similar to
#' readr::DataFrameCallback() except that it uses `dplyr::bind_rows()`
#' instead of `rbind()` so that it is faster.
#'
#'
#' @usage NULL
#' @format NULL
#' @name callback
#' @keywords internal
NULL

# Direct copy of readr::ChunkCallback
#' @usage NULL
#' @format NULL
#' @rdname callback
#' @export
HipChunkCallback <- R6::R6Class(
  "HipChunkCallback",
  private = list(
    callback = NULL
  ),
  public = list(
    initialize = function(callback) NULL,
    receive = function(data, index) NULL,
    continue = function() TRUE,
    result = function() NULL,
    finally = function() NULL
  )
)

# Direct copy of readr::SideEffectChunkCallback
#' @usage NULL
#' @format NULL
#' @rdname callback
#' @export
HipSideEffectChunkCallback <- R6::R6Class(
  "HipSideEffectChunkCallback", inherit = HipChunkCallback,
  private = list(
    cancel = FALSE
  ),
  public = list(
    initialize = function(callback) {
      check_callback_fun(callback)
      private$callback <- callback
    },
    receive = function(data, index) {
      result <- private$callback(data, index)
      private$cancel <- identical(result, FALSE)
    },
    continue = function() {
      !private$cancel
    }
  )
)

# Direct copy of readr::ListCallback
#' @usage NULL
#' @format NULL
#' @rdname callback
#' @export
HipListCallback <- R6::R6Class(
  "HipListCallback", inherit = HipChunkCallback,
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
      private$results
    },
    finally = function() {
      private$results <- list()
    }
  )
)

#' @usage NULL
#' @format NULL
#' @rdname callback
#' @export
HipDataFrameCallback <- R6::R6Class(
  "HipDataFrameCallback", inherit = HipChunkCallback,
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
