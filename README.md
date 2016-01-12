# Pcre2
This is a PHP extension that compiles a PCRE2 regular expression as separate step from the comparison step. Run time is faster than the builtin PCRE API for very large strings and multiple comparisons. It requires PCRE2 and PHP-CPP to be installed on the local system.

The extension makefile has been tested on Debian 8 with PHP 5.6.

##Usage
In PHP, the PCRE function:
```
$subject = 'abacadabra';
$result = pcre_replace('/a/', ' ', $subject); //  ' b c d br '
```
Is equivalent to:
```
$subject = 'abacadabra';
$replace = \Diskerror\Pcre2\Replace('a', ' ');
$result = $replace->exec($subject);  //  ' b c d br '
```
This will perform replacements on multiple strings with only one compile step, and without needing them to be gathered into a single array.

##PCRE2
The PCRE2 source can be found [here](http://www.pcre.org). Compiling this code requires that PCRE2 library be installed on the local system. The "configure" step was performed with these options:

```
./configure --enable-jit --enable-newline-is-anycrlf
make
sudo make install
```

##PHP-CPP
The [Copernica](http://www.copernica.com) PHP-CPP C++ library is used to build this extension. It might help to use [my version](https://github.com/diskerror/PHP-CPP) as it fixes some problems with the install step.
