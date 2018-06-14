library(hipread)

context("Read long")

NROW <- 9
NCOL <- 8
HNUM <- c("001", "001", "001", "001", "002", "002", "002", "003", "003")
PERMIX <- c(NA, "x", "1", "1", NA, "d", "1", NA, "d")
HIMPDEC <- c(1, NA, NA, NA, 4.5, NA, NA, 2.4, NA)

test_that("Can read a basic example", {
  actual <- readh_long(
    hipread_example("test-basic.dat"),
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

  expect_equal(nrow(actual), NROW)
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$hhnum, HNUM)
  expect_equal(actual$per_mix, PERMIX)
  expect_equal(actual$hh_impdbl, HIMPDEC)
})

test_that("Can read a basic gzipped example", {
  actual <- readh_long(
    hipread_example("test-basic.dat.gz"),
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
  expect_equal(nrow(actual), NROW)
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$hhnum, HNUM)
  expect_equal(actual$per_mix, PERMIX)
  expect_equal(actual$hh_impdbl, HIMPDEC)
})

test_that("Can skip in a basic example", {
  actual <- readh_long(
    hipread_example("test-basic.dat"),
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
    ),
    skip = 2
  )

  expect_equal(nrow(actual), NROW - 2)
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$hhnum, HNUM[-(1:2)])
  expect_equal(actual$per_mix, PERMIX[-(1:2)])
  expect_equal(actual$hh_impdbl, HIMPDEC[-(1:2)])
})

test_that("Can n_max in a basic example", {
  actual <- readh_long(
    hipread_example("test-basic.dat"),
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
    ),
    n_max = 4
  )

  expect_equal(nrow(actual), 4)
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$hhnum, HNUM[1:4])
  expect_equal(actual$per_mix, PERMIX[1:4])
  expect_equal(actual$hh_impdbl, HIMPDEC[1:4])
})

NROW <- 9
NCOL <- 3
VAR1 <- c("H", "P", "P", "P", "H", "P", "P", "H", "P")
test_that("Can read a rectangular example", {
  var_info <- list(
    list(
      start = c(1, 2, 4),
      width = c(1, 2, 1),
      var_pos = c(1, 2, 3)
    )
  )
  # names(var_info) <- "H"
  actual <- readh_long(
    hipread_example("test-basic.dat"),
    c("var1", "var2", "var3"),
    c("character", "character", "character"),
    1,
    0,
    var_info,
    list(
      list(trim_ws = TRUE),
      list(trim_ws = TRUE),
      list(trim_ws = TRUE)
    )
  )

  expect_equal(nrow(actual), NROW)
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$var1, VAR1)
})


NROW <- 300000
TEXT <- c("abc123xyz", "def456uvw", "ghi789rst", "jkl000opq")
NCOL <- 3
VAR1 <- rep(strtrim(TEXT, 3), NROW)

test_that("Can read a large gzipped example", {
  full_data <- rep(TEXT, NROW)
  temp_file <- tempfile(fileext = ".dat.gz")
  temp_conn <- gzfile(temp_file)

  writeLines(full_data, temp_conn)

  close(temp_conn)
  rm(full_data)

  actual <- readh_long(
    temp_file,
    c("var1", "var2", "var3"),
    c("character", "integer", "character"),
    1,
    0,
    list(
      list(
        start = c(1, 4, 7),
        width = c(3, 3, 3),
        var_pos = c(1, 2, 3)
      )
    ),
    list(
      list(trim_ws = TRUE),
      list(),
      list(trim_ws = TRUE)
    )
  )

  expect_equal(nrow(actual), NROW * length(TEXT))
  expect_equal(ncol(actual), NCOL)
  expect_equal(actual$var1, VAR1)
})


test_that("Default var_opts work", {
  specified <- readh_long(
    hipread_example("test-basic.dat"),
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
      list(trim_ws = TRUE),
      list(trim_ws = TRUE),
      list(imp_dec = 0L),
      list(imp_dec = 0L),
      list(),
      list(imp_dec = 0L),
      list(trim_ws = TRUE)
    )
  )

  default <- readh_long(
    hipread_example("test-basic.dat"),
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
    )
  )

  expect_equal(specified, default)
})
