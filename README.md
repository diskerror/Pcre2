# Pcre2
Version 0.1 of a PHP extension that compiles a PCRE2 regular expression separate step from the comparison step. Run time is faster than the builtin PCRE API for very large strings and multiple comparisons. Speed optimizations to come.

PCRE2 can be found [here](http://www.pcre.org) . Compiling this code requires that PCRE2 library be installed on the local system. The "configure" step was performed with these options: ```./configure --enable-jit --enable-newline-is-anycrlf```

The makefiles has been tested on Debian 8 with PHP 5.6.
