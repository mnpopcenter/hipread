% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/col_specs.R
\name{hip_rt}
\alias{hip_rt}
\title{Create a record type information object}
\usage{
hip_rt(start, width, warn_on_missing = TRUE)
}
\arguments{
\item{start}{Start position of the record type variable}

\item{width}{The width of the record type variable}

\item{warn_on_missing}{Whether to warn when encountering a
record type that is not specified}
}
\value{
A list, really only intended to be used internally by hipread
}
\description{
Create a record type information object for hipread to use
when reading hierarchical files. A width of 0 indicates that
the file is rectangular (eg a standard fixed width file).
}
