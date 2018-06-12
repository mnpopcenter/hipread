library(hipread)

context("Encodings work")

test_that("latin1 and utf-8 match", {
  expect_equal(
    readh_long(
      hipread_example("test-enc-latin1.dat"),
      c("v1", "v2"),
      c("character", "integer"),
      1,
      0,
      list(
        list(
          start = c(1, 4),
          width = c(3, 3),
          var_pos = c(1, 2)
        )
      ),
      list(
        list(trim_ws = TRUE),
        list()
      ),
      encoding = "latin1"
    ),
    readh_long(
      hipread_example("test-enc-utf8.dat"),
      c("v1", "v2"),
      c("character", "integer"),
      1,
      0,
      list(
        list(
          start = c(1, 4),
          width = c(3, 3),
          var_pos = c(1, 2)
        )
      ),
      list(
        list(trim_ws = TRUE),
        list()
      ),
      encoding = "UTF-8"
    )
  )
})

test_that("skip bom in utf-8 bom", {
  expect_equal(
    readh_long(
      hipread_example("test-enc-utf8.dat"),
      c("v1", "v2"),
      c("character", "integer"),
      1,
      0,
      list(
        list(
          start = c(1, 4),
          width = c(3, 3),
          var_pos = c(1, 2)
        )
      ),
      list(
        list(trim_ws = TRUE),
        list()
      ),
      encoding = "UTF-8"
    ),
    readh_long(
      hipread_example("test-enc-utf8bom.dat"),
      c("v1", "v2"),
      c("character", "integer"),
      1,
      0,
      list(
        list(
          start = c(1, 4),
          width = c(3, 3),
          var_pos = c(1, 2)
        )
      ),
      list(
        list(trim_ws = TRUE),
        list()
      ),
      encoding = "UTF-8"
    )

  )
})
