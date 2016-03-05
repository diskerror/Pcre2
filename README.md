# Pcre2
This is a PHP extension that compiles a PCRE2 regular expression as separate step from the comparison step. Run time is faster than the builtin PCRE API for very large strings and multiple comparisons. It requires PCRE2 and PHP-CPP to be installed on the local system.

The extension makefile has been tested on Debian 8 with PHP 5.6 and CentOS 6 with PHP 5.5.

##Usage
In PHP, the PCRE function:
```
$subject = 'abacadabra';
$result = pcre_replace('/a/', ' ', $subject); //  ' b c d br '

$result = pcre_match('/a/', $subject);  //  true

$matches = [];
$result = pcre_match('/a/', $subject, $matches);  //  true
print_r($matches);  //  ['a', 'a']
```
Is equivalent to:
```
$subject = 'abacadabra';
$replace = \Diskerror\Pcre2\Replace('a', ' ');
$result = $replace->exec($subject);  //  ' b c d br '

$hasMatch = \Diskerror\Pcre2\HasMatch('a');
$result = $hasMatch->exec($subject);  //  true

$matches = [];
$match = \Diskerror\Pcre2\Match('a');
$result = $match->exec($subject, $matches);
print_r($matches);  //  ['a', 'a']
```
This will perform replacements on multiple strings with only one compile step, and without needing them to be gathered into a single array.

## Requirements For Compiling
GCC, Make, and the standard libraries are required to build and install the custom extension, as is the PHP development libraries.

CentOS 6 requires at least devtoolset-2 to compile PHP-CPP.
```
 > cd /etc/yum.repos.d
 > wget http://people.centos.org/tru/devtools-2/devtools-2.repo
 > yum --enablerepo=testing-devtools-2-centos-6 install devtoolset-2-gcc devtoolset-2-gcc-c++
 > scl enable devtoolset-2 bash
```

###PCRE2
The PCRE2 source can be found [here](http://www.pcre.org). Compiling this code requires that PCRE2 library be installed on the local system.
```
 > ./configure --enable-jit --enable-newline-is-anycrlf
 > make
 > sudo make install
```

###PHP-CPP
The [Copernica](http://www.copernica.com) PHP-CPP library is used to build this extension. Use these commands:
```
 > make release
 > sudo make install
```
Just using ```make``` will create a slower debug version.
