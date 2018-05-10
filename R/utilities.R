show_progress <- function() {
  isTRUE(getOption("readr.show_progress")) && # user disables progress bar
    interactive() && # an interactive session
    is.null(getOption("knitr.in.progress")) # Not actively knitting a document
}
