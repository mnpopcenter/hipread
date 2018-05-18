library(hipread)

context("Value conversions")

test_that("whitespace double to NA", {
  actual <- readh_long(
    hipread_example("test-whitespace.dat"),
    c("rt", "var1", "var2"),
    c("character", "double", "double"),
    1,
    1,
    list(
      H = list(
        start = c(1, 2, 3),
        width = c(1, 1, 1),
        var_pos = c(1, 2, 3)
      ),
      P = list(
        start = c(1, 2, 3),
        width = c(1, 1, 1),
        var_pos = c(1, 2, 3)
      )
    ),
    list(
      list(trimws = TRUE),
      list(imp_dec = 0L),
      list(imp_dec = 1L)
    )
  )

  expect_true(all(is.na(actual$var1)), "double no implicit decimal")
  expect_true(all(is.na(actual$var2)), "double one implicit decimal")
})

test_that("whitespace double to NA", {
  actual <- readh_long(
    hipread_example("test-whitespace.dat"),
    c("rt", "var1"),
    c("character", "integer"),
    1,
    1,
    list(
      H = list(
        start = c(1, 2),
        width = c(1, 1),
        var_pos = c(1, 2)
      ),
      P = list(
        start = c(1, 2),
        width = c(1, 1),
        var_pos = c(1, 2)
      )
    ),
    list(
      list(trimws = TRUE),
      list()
    )
  )

  expect_true(all(is.na(actual$var1)), "integer no implicit decimal")
})
