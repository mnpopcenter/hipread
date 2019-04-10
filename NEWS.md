# hipread 0.2.0.9000
* progress bar will show by default even if readr hasn't been loaded (#12).

# hipread 0.2.0
* Added `yield` functions (`hipread_long_yield()` and `hipread_list_yield()`) which
  are another take on reading data in chunks that allow for more flexibility.

* Progress bar now ends at 100% instead of looking like the read was incomplete (#6)

* Several performance improvements

# hipread 0.1.1

* Fixes for platform-specific bugs revealed by CRAN checks (Solaris, UBSAN, Fedora)
