library(hipread)

context("Read frequencies")

NVARS <- 8
HNUM_FREQ_SORTED <- c("003" = 2, "002" = 3, "001" = 4)
PERMIX_FREQ_SORTED <- c("x" = 1, "d" = 2, "1" = 3)

test_that("Can read a basic example", {
  actual <- readh_freqs(
    hipread_example("test-basic.dat"),
    c("rt", "hhnum", "hh_char", "hh_dbl", "hh_impdbl", "pernum", "per_dbl", "per_mix"),
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

  expect_equal(length(actual), NVARS)
  expect_equal(sort(actual$hhnum), HNUM_FREQ_SORTED)
  expect_equal(sort(actual$per_mix), PERMIX_FREQ_SORTED)
})
