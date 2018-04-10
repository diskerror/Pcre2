# Pcre2
This is a PHP extension that compiles a PCRE2 regular expression as separate step from the comparison step. Run time is faster than the builtin PCRE API for very large strings and multiple comparisons. It requires [PCRE2](http://www.pcre.org) and [PHP-CPP](http://www.php-cpp.com/) to be installed on the local system.

The extension makefile has been tested on Debian 8 with PHP 5.6 and CentOS 6 with PHP 5.5.

This current version (0.2.2) has been updated to work with PHP-CPP version 2 and the makefile has been updated to work with many of the various PHP configuration paths.

## Usage
In PHP, the PCRE function:
```
$subject = 'abacadabra';
$result = preg_replace('/a/', ' ', $subject); //  ' b c d br '

$result = preg_match('/a/', $subject);  //  true

$matches = [];
$result = preg_match('/a/', $subject, $matches);  //  true
print_r($matches);  //  ['a', 'a']
```
Is equivalent to:
```
$subject = 'abacadabra';
$replace = new Diskerror\Pcre2\Replace('a', ' ');
echo '"', $replace->hasMatch($subject), "\"\n";  //  " b c d br "

$hasMatch = new Diskerror\Pcre2\HasMatch('a');
var_dump($hasMatch->match($subject));  //  bool(true)

$matches = [];
$match = new Diskerror\Pcre2\Match('a');
$result = $match->replace($subject, $matches);
print_r($matches);  //  Array([0] => a)
```
This will perform replacements on multiple strings with only one compile step, and without needing them to be gathered into a single array.

## Requirements For Compiling
GCC, Make, and the standard libraries are required to build and install the custom extension, as is the PHP development libraries.

CentOS 6 requires at least devtoolset-2 to compile [PHP-CPP](http://www.php-cpp.com/).
```
 > cd /etc/yum.repos.d
 > wget http://people.centos.org/tru/devtools-2/devtools-2.repo
 > yum --enablerepo=testing-devtools-2-centos-6 install devtoolset-2-gcc devtoolset-2-gcc-c++
 > scl enable devtoolset-2 bash
```

### PCRE2
The PCRE2 source can be found [here](http://www.pcre.org).
```
 > ./configure --enable-jit --enable-newline-is-anycrlf
 > make
 > sudo make install
```

### PHP-CPP
The Copernica [PHP-CPP](http://www.php-cpp.com/) library is used to build this extension.
```
 > make release
 > sudo make install
```
Just using ```make``` will create a slower debug version.
