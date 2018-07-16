check_file <- function(file) {
  file <- enc2native(normalizePath(file))
  if (!file.exists(file)) stop(paste0("Could not find file: ", file))
  file
}

is_gzip_compression <- function(comp, file) {
  if (is.null(comp)) {
    return(tools::file_ext(file) == "gz")
  } else if (length(comp) != 1) {
    stop("Expected length 1 argument to compression but got ", length(comp))
  } else if (comp == "txt") {
    return(FALSE)
  } else if (comp == "gz") {
    return(TRUE)
  } else {
    stop(paste0("Unexpected compression type ", comp))
  }
}

get_var_names <- function(var_info) {
  Reduce(function(x, y) {union(x, y$col_names)}, var_info, character(0))
}

get_var_pos <- function(var_info, var_names = NULL) {
  if (is.null(names(var_info))) {
    if (length(var_info) == 1) {
      names(var_info) <- "rectangular"
    } else {
      stop("Variable information requires names if there's more than 1 record type")
    }
  }

  out <- lapply(var_info, function(vp) {
    list(
      start = vp$start,
      width = vp$end - vp$start,
      var_pos = match(vp$col_names, var_names) - 1,
      max_end = max(vp$end)
    )
  })
  names(out) <- names(var_info)

  out
}

get_var_types <- function(var_info, var_names) {
  var_types <- lapply(var_info, function(x) dplyr::select_at(x, c("col_names", "col_types")))
  names(var_types) <- names(var_info)
  var_types <- dplyr::bind_rows(var_types, .id = "rectype")

  check_option_consistency(var_types, "col_types")
  var_types <- dplyr::select(var_types, -.data$rectype)
  var_types <- dplyr::distinct(var_types)

  var_types$col_types[match(var_types$col_names, var_names)]
}

get_var_opts <- function(var_info, var_names) {
  var_opts <- lapply(
    var_info, function(x) dplyr::select_at(x, c("col_names", "col_types", "trim_ws", "imp_dec"))
  )
  names(var_opts) <- names(var_info)
  var_opts <- dplyr::bind_rows(var_opts, .id = "rectype")
  var_opts <- dplyr::mutate(
    var_opts,
    trim_ws = ifelse(.data$col_types == "character", .data$trim_ws, NA),
    imp_dec = ifelse(.data$col_types == "double", .data$imp_dec, NA)
  )

  check_option_consistency(var_opts, "trim_ws")
  check_option_consistency(var_opts, "imp_dec")

  lapply(var_names, function(vvv) {
    list(
      trim_ws = var_opts$trim_ws[var_opts$col_names == vvv][[1]],
      imp_dec = var_opts$imp_dec[var_opts$col_names == vvv][[1]]
    )
  })
}

check_freq_args <- function(var_names, var_pos_info) {
  checks <- lapply(names(var_pos_info), function(rt_name) {
    if (any(var_pos_info[[rt_name]]$var_pos + 1 > length(var_names))) stop(paste0(
      "For rectype ", rt_name, " variable positions exceeds number of variables."
    ))
  })
  invisible(NULL)
}

is_integerish <- function(x) {
  all.equal(x, as.integer(x))
}

check_skip <- function(x) {
  if (length(x) > 1) stop("skip must be length one")
  if (!is_integerish(x) || x < 0) stop("skip must be a positive integer")

  as.integer(x)
}

check_n_max <- function(x) {
  if (length(x) > 1) stop("n_max must be length one")
  if (is.infinite(x) | x < 0) x <- .Machine$integer.max
  if (!is_integerish(x) || x < 0) stop("n_max must be a positive integer")

  as.integer(x)
}

standardize_col_types <- function(x) {
  out <- dplyr::case_when(
    x %in% c("c", "character") ~ "character",
    x %in% c("d", "double") ~ "double",
    x %in% c("i", "integer") ~ "integer",
    TRUE ~ NA_character_
  )

  if (any(is.na(out))) {
    bad_types <- unique(x[is.na(out)])
    stop("Unrecognized column types: ", paste(bad_types, collapse = ", "))
  }
  out
}

add_level_to_rect <- function(x) {
  if (inherits(x, "hip_pos")) x <- list(rectangular = x)
  x
}

check_option_consistency <- function(opts, opt_name) {
  opts <- dplyr::group_by_at(opts, c("col_names"))
  opts <- dplyr::mutate(opts, num_unique_opts = length(unique(.data[[opt_name]])))

  if (any(opts$num_unique_opts > 1)) {
    bad_types <- dplyr::filter(opts, .data$num_unique_opts > 1)
    bad_types <- dplyr::summarize(
      bad_types,
      message = paste0(
        .data$col_names[1], "(", paste(.data$rectype, "-", .data[[opt_name]], collapse = " & "), ")"
      )
    )
    stop(paste0(
      "Varibles with the same name must have the same ", opt_name, " across all record ",
      "types but these do not: ", paste(bad_types$message, collapse = ", ")
    ))
  }
}


get_vinfo_col_as_list <- function(var_info, col) {
  out <- lapply(var_info, function(x) {
    x[[col]]
  })
  names(out) <- names(var_info)
  out
}


get_var_opts_list <- function(var_info) {
  out <- lapply(var_info, function(x) {
    opts <- dplyr::select_at(x, c("col_names", "col_types", "trim_ws", "imp_dec"))
    opts <- dplyr::mutate(
      opts,
      trim_ws = ifelse(.data$col_types == "character", .data$trim_ws, NA),
      imp_dec = ifelse(.data$col_types == "double", .data$imp_dec, NA)
    )

    out <- lapply(seq_len(nrow(opts)), function(iii) {
      list(trim_ws = opts$trim_ws[iii], imp_dec = opts$imp_dec[iii])
    })
    names(out) <- opts$col_names
    out
  })
  names(out) <- names(var_info)
  out
}
