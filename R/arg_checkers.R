create_rt_info <- function(start, width) {
  if (length(start) > 1) stop(paste0(
    "rectype start must be a single integer, but is of size ", length(start)
  ))

  if (length(width) > 1) stop(paste0(
    "rectype width must be a single integer, but is of size ", length(width)
  ))

  if (!is_integerish(start)) stop(paste0(
    "rectype start must be an integer but is ", start
  ))

  if (!is_integerish(width)) stop(paste0(
    "rectype width must be an integer but is ", width
  ))

  if (start < 1) stop(paste0(
    "rectype start must be greater than 1 but is ", start
  ))

  list(start = start - 1, width = width)
}

check_file <- function(file) {
  if (!file.exists(file)) stop(paste0("Could not find file: ", file))
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

check_long_var_pos_info <- function(var_pos) {
  if (is.null(names(var_pos))) stop("Variable position information requires names")
  required_var_names <- c("start", "width", "var_pos")
  out <- lapply(names(var_pos), function(rt_name) {
    rt <- var_pos[[rt_name]]
    missing_var <- setdiff(names(rt), required_var_names)
    if (length(missing_var) > 0) {
      stop(paste0(
        "Variable position information for record type ", rt_name, " is missing ",
        paste(missing_var, collapse = ", ")
      ))
    }

    var_lens <- vapply(required_var_names, function(x) length(rt[[x]]), integer(1))
    if (!all(var_lens == var_lens[1])) stop(paste0(
      "Not all positon information for record type ", rt_name, " is the same length ",
      paste(required_var_names, " (", var_lens, ")", collapse = ", ")
    ))
    if (length(rt$start) > 0) {
      if (!is_integerish(rt$start)) stop(paste0(
        "start must be integers but is ", paste(rt$start, collapse = ", ")
      ))
      if (!is_integerish(rt$width)) stop(paste0(
        "width must be integers but is ", paste(rt$width, collapse = ", ")
      ))
      if (!is_integerish(rt$var_pos)) stop(paste0(
        "var_pos must be integers but is ", paste(rt$var_pos, collapse = ", ")
      ))
      if (any(rt$start < 1)) stop(paste0(
        "For rectype ", rt_name, " some variable starts are less than 1"
      ))
      if (any(rt$width < 0)) stop(paste0(
        "For rectype ", rt_name, " some variable widths are less than 0"
      ))
      if (any(rt$var_pos < 1)) stop(paste0(
        "For rectype ", rt_name, " some variable positions are less than 1"
      ))

      # C++ Is 0 indexed
      rt$start <- rt$start - 1
      rt$var_pos <- rt$var_pos - 1
      rt$max_end <- max(rt$start + rt$width)
    } else {
      rt$max_end <- integer(0)
    }
    rt
  })

  names(out) <- names(var_pos)
  out
}

check_var_opts <- function(var_opts, var_types) {
  lapply(seq_along(var_opts), function(iii) {
    vt <- var_types[[iii]]
    opt <- var_opts[[iii]]
    if (vt == "character" && !("trim_ws" %in% names(opt))) {
      opt$trim_ws <- TRUE
    } else if (vt == "double" && !("imp_dec" %in% names(opt))) {
      opt$imp_dec <- 0
    }
    opt
  })
}

check_long_arg_lengths <- function(var_names, var_types, var_pos_info, var_opts) {
  all_lengths <- lengths(list(var_names, var_types, var_opts))
  if (length(unique(all_lengths)) != 1) stop(paste0(
    "variable names (length: ", all_lengths[1], "), variable types (", all_lengths[2],
    ") and variable options (", all_lengths[3], ") must all be the same length."
  ))

  checks <- lapply(names(var_pos_info), function(rt_name) {
    if (any(var_pos_info[[rt_name]]$var_pos + 1 > all_lengths[1])) stop(paste0(
      "For rectype ", rt_name, " variable positions exceeds number of variables."
    ))
  })
  invisible(NULL)
}

is_integerish <- function(x) {
  all.equal(x, as.integer(x))
}

