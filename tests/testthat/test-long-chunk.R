library(hipread)

context("Read chunk long")

FILTERED_NROW <- 6
NCOL <- 8
FILTERED_HNUM <- c("001", "001", "001", "001", "003", "003")

test_that("Can read a basic example", {
  actual <- hipread:::readh_long_chunked(
    hipread_example("test-basic.dat"),
    HipDataFrameCallback$new(function(x, pos) x[x$hhnum != "002", ]),
    4,
    c("rt", "hhnum", "hh_char", "hh_dbl", "hh_impdbl", "pernum", "per_dbl", "per_mix"),
    c("character", "character", "character", "double", "double", "integer", "double", "character"),
    1,
    1,
    list(
      H = list(
        start = c(1, 2, 5, 8, 11),
        width = c(1, 3, 3, 3, 2),
        var_pos = c(1, 2, 3, 4, 5)
      ),
      P = list(
        start = c(1, 2, 5, 6, 9),
        width = c(1, 3, 1, 3, 1),
        var_pos = c(1, 2, 6, 7, 8)
      )
    ),
    list(
      list(trim_ws = TRUE),
      list(trim_ws = FALSE),
      list(trim_ws = TRUE),
      list(imp_dec = 0L),
      list(imp_dec = 1L),
      list(),
      list(imp_dec = 0L),
      list(trim_ws = TRUE)
    )
  )

  expect_equal(nrow(actual), FILTERED_NROW)
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$hhnum, FILTERED_HNUM)
})

test_that("Can read a basic gzipped example", {
  actual <- hipread:::readh_long_chunked(
    hipread_example("test-basic.dat.gz"),
    HipDataFrameCallback$new(function(x, pos) x[x$hhnum != "002", ]),
    4,
    c("rt", "hhnum", "hh_char", "hh_dbl", "hh_impdbl", "pernum", "per_dbl", "per_mix"),
    c("character", "character", "character", "double", "double", "integer", "double", "character"),
    1,
    1,
    list(
      H = list(
        start = c(1, 2, 5, 8, 11),
        width = c(1, 3, 3, 3, 2),
        var_pos = c(1, 2, 3, 4, 5)
      ),
      P = list(
        start = c(1, 2, 5, 6, 9),
        width = c(1, 3, 1, 3, 1),
        var_pos = c(1, 2, 6, 7, 8)
      )
    ),
    list(
      list(trim_ws = TRUE),
      list(trim_ws = FALSE),
      list(trim_ws = TRUE),
      list(imp_dec = 0L),
      list(imp_dec = 1L),
      list(),
      list(imp_dec = 0L),
      list(trim_ws = TRUE)
    )
  )
  expect_equal(nrow(actual), FILTERED_NROW)
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$hhnum, FILTERED_HNUM)
})
