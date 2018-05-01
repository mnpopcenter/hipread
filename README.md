# hipread

hipread (hierarchical IPUMS readr) adapts code from readr to better serve the
ipumsr package which reads hierarchical* fixed width files with (generally) high
quality column metadata. Because of the higher quality metadata, I've been able
to simplify some of the code from readr, and then add the extra complexity of
the hierarchical data.

This package was only split out of the main ipumsr codebase to clarify the
licensing terms (readr is GPL2 and I felt I was borrowing too much code from
readr to fit under ipumsr's MPL license). I do not expect that this will be
super useful for too many people, so the documentation is a little bit light.
But, if you are interested and find something confusing, please let me know!

## Installation

Install the development version from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("mnpopcenter/ipumsr/hipread")
```

*Hierarchcical meaning that there can be multiple record types with different
column specifications in a single file and these record types are interwoven.
For example, the CPS data from an IPUMS extract has household records and person
records, each with their own set of variables.
